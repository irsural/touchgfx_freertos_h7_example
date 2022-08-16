#ifndef AUDIO_PLAY_H
#define AUDIO_PLAY_H

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_sai.h>
#include <stm32h743i_eval_audio.h>

class audio_play_t
{
private:
  SAI_HandleTypeDef m_sai_handle;
  DMA_HandleTypeDef m_dma_sai_handle;
  WM8994_Object_t m_wm8994;
  WM8994_Init_t m_wm8994_init;
  AUDIO_Drv_t* m_audio_driver;

  void sai_init();
  void audio_codec_init();

  bool start_sending_data(uint8_t* a_data, size_t a_data_size);
  bool pause_sending_data();
  bool resume_sending_data();
  bool stop_sending_data();
public:
  audio_play_t();

  DMA_HandleTypeDef* dma_sai_handle();

  bool play(uint8_t* a_data, size_t a_data_size);
  bool pause();
  bool resume();
  bool stop();

  void set_volume(int a_volume);
};

#endif // AUDIO_PLAY_H
