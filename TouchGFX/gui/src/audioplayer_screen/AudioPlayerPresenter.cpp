#include <gui/audioplayer_screen/AudioPlayerView.hpp>
#include <gui/audioplayer_screen/AudioPlayerPresenter.hpp>
#include <vector>

#include "cfg.h"
#include "codecvt_oem866.h"
#include "read_dir_thread.h"

AudioPlayerPresenter::AudioPlayerPresenter(AudioPlayerView& v)
    : view(v),
    m_read_folred_queue(xQueueCreate(5, sizeof(char*))),
    m_read_wav_files_thread(nullptr),
    m_play_wav_cmd_queue(xQueueCreate(5, sizeof(play_wav_thread_t::cmd_t))),
    m_percents_played_queue(xQueueCreate(5, sizeof(double))),
    m_play_wav_thread(nullptr),
    m_fft_samples_ready_smph(xSemaphoreCreateBinary()),
    m_fft_samples_processed_smph(xSemaphoreCreateBinary()),
    m_fft_done_smph(xSemaphoreCreateBinary()),
    m_fft_thread(nullptr)
{

}

void AudioPlayerPresenter::activate()
{
  view.show_initializing();
  initialize_file_system();
}

void AudioPlayerPresenter::initialize_file_system()
{
  m_read_wav_files_thread.reset(new read_dir_thread_t(m_read_folred_queue, ".wav", model->get_gui_thread()));
  create_thread(*m_read_wav_files_thread.get(), "read_wavs", osPriorityAboveNormal, 512);
  xQueueSend(m_read_folred_queue, static_cast<const void*>(&fatfs::sd::drive), portMAX_DELAY);

  m_fft_thread.reset(new fft_thread_t(m_fft_samples_ready_smph, m_fft_samples_processed_smph, m_fft_done_smph));
  create_thread(*m_fft_thread.get(), "fft", osPriorityBelowNormal, 1024);

  m_play_wav_thread.reset(new play_wav_thread_t(m_play_wav_cmd_queue, m_percents_played_queue));
  m_play_wav_thread->set_up_fft(m_fft_samples_ready_smph, m_fft_samples_processed_smph,
    m_fft_thread->get_samlpes_buffer());
  create_thread(*m_play_wav_thread.get(), "play_wav", osPriorityHigh, 512);
}

void AudioPlayerPresenter::deactivate()
{
  vQueueDelete(m_read_folred_queue);
}

void AudioPlayerPresenter::set_track_position(int a_track_position)
{
  play_wav_thread_t::cmd_t cmd = { play_wav_thread_t::cmd_type_t::set_position, a_track_position };
  xQueueSend(m_play_wav_cmd_queue, static_cast<const void*>(&cmd), 0);
}

void AudioPlayerPresenter::set_volume(int a_volume)
{
  play_wav_thread_t::cmd_t cmd = { play_wav_thread_t::cmd_type_t::set_volume, a_volume };
  xQueueSend(m_play_wav_cmd_queue, static_cast<const void*>(&cmd), 0);
}

void AudioPlayerPresenter::presenter_tick()
{
  if (ulTaskNotifyTake(pdTRUE, 0)) {
    for (auto& file_name: m_read_wav_files_thread->get_names()) {
      view.add_element_to_playlist(file_name);
    }
    m_read_wav_files_thread->clear_names();
    view.close_initializing();
  }

  double percents_played = 0;
  if (xQueueReceive(m_percents_played_queue, static_cast<void*>(&percents_played), 0) == pdTRUE) {
    if (percents_played == play_wav_thread_t::eof_value) {
      view.set_track_slider(0);
      view.set_pause_state();
    } else {
      view.set_track_slider(percents_played);
    }
  }

  static uint32_t prev_time = 0;
  if (HAL_GetTick()> prev_time + 1) {
    prev_time = HAL_GetTick();
    if (xSemaphoreTake(m_fft_done_smph, 0) == pdTRUE) {
      view.graph().clear();
      for (auto harmonic: m_fft_thread->get_harmonics()) {
        view.graph().addDataPoint(harmonic);
        view.graph().invalidate();
      }
    }
  }
}

void AudioPlayerPresenter::start_playback(const std::vector<touchgfx::Unicode::UnicodeChar>& a_song_name)
{
  m_play_wav_thread->set_wav_file(unicode_to_oem866(a_song_name));

  play_wav_thread_t::cmd_t cmd = { play_wav_thread_t::cmd_type_t::play, 0 };
  xQueueSend(m_play_wav_cmd_queue, static_cast<const void*>(&cmd), 0);

//  DBG_MSG("play " << unicode_to_oem866(a_song_name));

  view.set_play_state();
}

bool AudioPlayerPresenter::resume()
{
  play_wav_thread_t::cmd_t cmd { play_wav_thread_t::cmd_type_t::resume, 0 };
  xQueueSend(m_play_wav_cmd_queue, static_cast<const void*>(&cmd), 0);
  return true;
}

void AudioPlayerPresenter::pause()
{
  play_wav_thread_t::cmd_t cmd = { play_wav_thread_t::cmd_type_t::pause, 0 };
  xQueueSend(m_play_wav_cmd_queue, static_cast<const void*>(&cmd), 0);
}
