#include <gui/audioplayer_screen/AudioPlayerView.hpp>
#include <gui/audioplayer_screen/AudioPlayerPresenter.hpp>
#include <vector>

#include "cfg.h"
#include "codecvt_oem866.h"
#include "read_dir_thread.h"

AudioPlayerPresenter::AudioPlayerPresenter(AudioPlayerView& v)
    : view(v),
    m_read_folred_queue(xQueueCreate(5, sizeof(char*))),
    m_read_wav_files_thread(nullptr)
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

  create_thread(*m_read_wav_files_thread.get(), "read_wav", osPriorityAboveNormal, 512);
  xQueueSend(m_read_folred_queue, static_cast<const void*>(&fatfs::sd::drive), portMAX_DELAY);
}

void AudioPlayerPresenter::deactivate()
{
  vQueueDelete(m_read_folred_queue);
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
}

void AudioPlayerPresenter::start_playback(const std::vector<touchgfx::Unicode::UnicodeChar>& a_song_name)
{
  DBG_MSG("play " << unicode_to_oem866(a_song_name));
}
