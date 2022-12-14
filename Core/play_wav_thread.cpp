#include "play_wav_thread.h"
#include "codecvt_oem866.h"
#include "cfg.h"


enum class dma_callback_t {
  half_complete,
  complete,
};

static QueueHandle_t dma_queue = nullptr;

play_wav_thread_t::play_wav_thread_t(QueueHandle_t a_cmd_queue, SemaphoreHandle_t a_percents_played_smph) :
  m_cmd_queue(a_cmd_queue),
  m_file(nullptr),
  m_current_cmd_type(cmd_type_t::none),
  // ???? ????????? ????????? wav, ??????? ?????? ??????? ? ???????
  m_wav_header_size(0x12C),
  m_audio_buffer(static_cast<uint8_t*>(std::aligned_alloc(32, m_audio_buffer_size))),
  m_play_state(play_state_t::stopped),
  m_wav_name(),
  m_already_read_data_pos(0),
  m_track_size(0),
  m_bytes_read(0),
  m_bytes_played_percents(0),
  m_percents_played_queue(a_percents_played_smph),
  m_samples_processed_semph(nullptr),
  m_samples_ready_semph(nullptr),
  mp_fft_samples(nullptr),
  m_next_make_fft_time(0)
{
  dma_queue = xQueueCreate(5, sizeof(dma_callback_t));
}

play_wav_thread_t::~play_wav_thread_t()
{
  vQueueDelete(dma_queue);
  vTaskDelete(m_handle);
}

void play_wav_thread_t::set_wav_file(std::string a_filename)
{
  m_wav_name = std::move(a_filename);
}

const std::string& play_wav_thread_t::get_wav_file()
{
  return m_wav_name;
}

bool play_wav_thread_t::play()
{
  bool file_opened = open_wav_file(m_wav_name);
  if (file_opened) {
    return cfg_t::instance().audio_player.play(m_audio_buffer, m_audio_buffer_size_samples);
  } else {
    return false;
  }
}

void play_wav_thread_t::stop()
{
  cfg_t::instance().audio_player.stop();
}

bool play_wav_thread_t::resume()
{
  return cfg_t::instance().audio_player.resume();
}

void play_wav_thread_t::pause()
{
  cfg_t::instance().audio_player.pause();
}

play_wav_thread_t::play_state_t play_wav_thread_t::get_play_state()
{
  return m_play_state;
}

bool play_wav_thread_t::open_wav_file(const std::string& a_filename)
{
  if (m_wav_name != "") {
    m_file.reset(new fatfs::file_t(a_filename.c_str(), FA_READ));
    m_file->seek(m_wav_header_size);
    m_file->read(m_audio_buffer, m_audio_buffer_size);

    m_track_size = m_file->size() - m_wav_header_size;
    m_bytes_read = m_audio_buffer_size_samples;
    m_bytes_played_percents = static_cast<double>(m_bytes_read) / m_track_size * 100;

    if (m_file->get_error() != FR_OK) {
      DBG_MSG("open_wav_file open file error");
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

void play_wav_thread_t::set_up_fft(SemaphoreHandle_t a_samples_ready_semph, SemaphoreHandle_t a_samples_processed_semph,
  std::vector<std::complex<float>>* ap_fft_samples)
{
  m_samples_ready_semph = a_samples_ready_semph;
  m_samples_processed_semph = a_samples_processed_semph;
  mp_fft_samples = ap_fft_samples;

  // ?? ??? ?????????? ?????? ?? ???????? ??????? (m_audio_buffer_size_samples ????),
  // ?????? ????? = 2 ?????, ??? ????? ?????? ?????? ?????
  assert(mp_fft_samples->size() == m_audio_buffer_size_samples / 2 / 2);
}

void play_wav_thread_t::task()
{
  dma_callback_t dma_cb;
  cmd_t cmd = { cmd_type_t::none, 0 };
  int miss = 0;

  while(1) {
    size_t queue_wait_time = m_current_cmd_type == cmd_type_t::none ? portMAX_DELAY : 0;
    if (xQueueReceive(m_cmd_queue, static_cast<void*>(&cmd), queue_wait_time) == pdTRUE) {

      m_current_cmd_type = cmd.type;

      switch(m_current_cmd_type) {
        case cmd_type_t::play: {
          if (m_play_state != play_state_t::stopped) {
            m_play_state = play_state_t::stopped;
            stop();
          }
          if (play()) {
            m_play_state = play_state_t::playing;
          } else {
            m_current_cmd_type = cmd_type_t::none;
          }
        } break;

        case cmd_type_t::resume: {
          if (m_play_state == play_state_t::paused) {
            if (resume()) {
              m_play_state = play_state_t::playing;
            } else {
              m_current_cmd_type = cmd_type_t::none;
            }
          } else if (m_play_state == play_state_t::stopped){
            if (play()) {
              m_play_state = play_state_t::playing;
            } else {
              m_current_cmd_type = cmd_type_t::none;
            }
          } else {
            m_current_cmd_type = cmd_type_t::none;
          }
        } break;

        case cmd_type_t::pause: {
          pause();
          m_play_state = play_state_t::paused;
          m_current_cmd_type = cmd_type_t::none;
        } break;

        case cmd_type_t::set_position: {
          if (m_file != nullptr) {
            // cmd.value ? ????????? ?? 0 ?? 1000
            size_t new_position = static_cast<size_t>(cmd.value / 1000. * m_track_size);
            new_position = new_position - (new_position % 512);
            m_file->seek(new_position);

            m_bytes_read = 0;
            m_bytes_played_percents = static_cast<double>(new_position) / m_track_size * 100;

          } else {
            m_current_cmd_type = cmd_type_t::none;
          }
        } break;

        case cmd_type_t::set_volume: {
          cfg_t::instance().audio_player.set_volume(cmd.value);
          if (m_play_state != play_state_t::playing) {
            m_current_cmd_type = cmd_type_t::none;
          }
        } break;
      }
    }

    if (m_play_state == play_state_t::playing) {
      if (xQueueReceive(dma_queue, static_cast<void*>(&dma_cb), portMAX_DELAY) == pdTRUE) {

        if (dma_cb == dma_callback_t::complete) {
          m_already_read_data_pos = m_audio_buffer_size_samples;
        } else { // dma_callback_t::half_complete
          m_already_read_data_pos = 0;
        }

        using namespace fatfs;
        file_t::result_t read_result = m_file->read(m_audio_buffer + m_already_read_data_pos, m_audio_buffer_size_samples);

        m_bytes_read += m_audio_buffer_size_samples;
        if (m_bytes_read > send_played_length_frequency) {
          m_bytes_played_percents += static_cast<double>(m_bytes_read) / m_track_size * 100;
          m_bytes_read = 0;

          xQueueSend(m_percents_played_queue, static_cast<void*>(&m_bytes_played_percents), 0);
        }

        if (m_samples_ready_semph != nullptr) {
          uint32_t current_time = HAL_GetTick();
          if (m_next_make_fft_time <= current_time) {
            m_next_make_fft_time = current_time + make_fft_period_ms;
            if (xSemaphoreTake(m_samples_processed_semph, 0) == pdTRUE) {
              int16_t* samples_buffer = reinterpret_cast<int16_t*>(m_audio_buffer + m_already_read_data_pos);
              for(size_t i = 0, j = 0; i < m_audio_buffer_size_samples / 2; i += 2, ++j) {
                (*mp_fft_samples)[j] = std::complex<float>(static_cast<float>(samples_buffer[i]), 0);
              }
              xSemaphoreGive(m_samples_ready_semph);
            } else {
              ++miss;
            }
          }
        }

        if(read_result != file_t::result_t::success) {

          if (read_result == file_t::result_t::error) {
            DBG_MSG("Audio read error! Play is stoped");
          }
          xQueueReset(dma_queue);
          m_file.reset();
          stop();
          m_play_state = play_state_t::stopped;
          DBG_MSG("miss: " << miss);
          xQueueSend(m_percents_played_queue, static_cast<const void*>(&eof_value), 0);
          m_next_make_fft_time = 0;
          m_current_cmd_type = cmd_type_t::none;
        }

        if (uxQueueMessagesWaiting(dma_queue) > 1) {
  //      DBG_MSG("Audio read is too slow!");
        }
      }
    }
  }
}

extern "C" {

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
  dma_callback_t dma_cb = dma_callback_t::complete;
  xQueueSendFromISR(dma_queue, static_cast<const void*>(&dma_cb), 0);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  dma_callback_t dma_cb = dma_callback_t::half_complete;
  xQueueSendFromISR(dma_queue, static_cast<const void*>(&dma_cb), 0);
}

}
