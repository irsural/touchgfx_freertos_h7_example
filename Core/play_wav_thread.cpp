#include "play_wav_thread.h"
#include "codecvt_oem866.h"
#include "cfg.h"


enum class dma_callback_t {
  half_complete,
  complete,
};

static QueueHandle_t dma_queue = nullptr;

play_wav_thread_t::play_wav_thread_t(QueueHandle_t a_cmd_queue) :
  m_cmd_queue(a_cmd_queue),
  m_file(nullptr),
  m_current_cmd(cmd_t::none),
  // Лень разбирать заголовки wav, поэтому просто возьмем с запасом
  m_wav_header_size(0x12C),
  m_audio_buffer(static_cast<uint8_t*>(std::aligned_alloc(32, m_audio_buffer_size))),
  m_playing(false),
  m_wav_name(),
  m_already_read_data_pos(0)
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

void play_wav_thread_t::play()
{
  cfg_t::instance().audio_player.start_sending_data(m_audio_buffer, m_audio_buffer_size / 2);
  cfg_t::instance().audio_player.resume();
}

void play_wav_thread_t::pause()
{
  cfg_t::instance().audio_player.pause();
  cfg_t::instance().audio_player.stop_sending_data();
}

bool play_wav_thread_t::open_wav_file(const std::string& a_filename)
{
  m_file.reset(new fatfs::file_t(a_filename.c_str(), FA_READ));
  m_file->seek(m_wav_header_size);
  m_file->read(m_audio_buffer, m_audio_buffer_size);

  if (m_file->get_error() != FR_OK) {
    DBG_MSG("open_wav_file open file error");
    return false;
  } else {
    return true;
  }
}



void play_wav_thread_t::task()
{
  dma_callback_t dma_cb;
  cmd_t cmd = cmd_t::none;

  while(1) {
    size_t queue_wait_time = m_current_cmd == cmd_t::none ? portMAX_DELAY : 0;
    if (xQueueReceive(m_cmd_queue, static_cast<void*>(&cmd), queue_wait_time) == pdTRUE) {

      m_current_cmd = cmd;

      switch(m_current_cmd) {
        case cmd_t::play: {
          bool file_opened = open_wav_file(m_wav_name);
          if (file_opened) {
            if (m_playing) {
              pause();
            }
            m_playing = true;
            play();
          }
        } break;

        case cmd_t::pause: {
          pause();
          m_playing = false;
          m_current_cmd = cmd_t::none;
        } break;
      }
    }

    if (xQueueReceive(dma_queue, static_cast<void*>(&dma_cb), portMAX_DELAY) == pdTRUE) {

      if (dma_cb == dma_callback_t::complete) {
        m_already_read_data_pos = m_audio_buffer_size / 2;
      } else { // dma_callback_t::half_complete
        m_already_read_data_pos = 0;
      }

      using namespace fatfs;
      file_t::result_t read_result = m_file->read(m_audio_buffer + m_already_read_data_pos, m_audio_buffer_size / 2);

      if(read_result != file_t::result_t::success) {

        if (read_result == file_t::result_t::error) {
          DBG_MSG("Audio read error! Play is stoped");
        }
        xQueueReset(dma_queue);
        pause();
        m_current_cmd = cmd_t::none;
      }

      if (uxQueueMessagesWaiting(dma_queue) > 1) {
//      DBG_MSG("Audio read is too slow!");
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
