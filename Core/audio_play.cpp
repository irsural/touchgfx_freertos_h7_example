#include "audio_play.h"

#include <stm32h743i_eval_bus.h>
#include "defs.h"


audio_play_t::audio_play_t() :
  m_sai_handle{0},
  m_dma_sai_handle{0},
  m_wm8994{0},
  m_wm8994_init{0},
  m_audio_driver(nullptr)
{
  sai_init();
  audio_codec_init();
}

bool audio_play_t::play(uint8_t* a_data, size_t a_data_size)
{
  start_sending_data(a_data, a_data_size);
  if (m_audio_driver->Play(&m_wm8994) < 0) {
    DBG_MSG("play_wav_thread_t Play error");
    return false;
  } else {
    return true;
  }
}

bool audio_play_t::pause()
{
  if (m_audio_driver->Pause(&m_wm8994) < 0) {
    DBG_MSG("play_wav_thread_t Pause error");
    return false;
  } else {
    return pause_sending_data();
  }
}

bool audio_play_t::resume()
{
  resume_sending_data();
  if (m_audio_driver->Resume(&m_wm8994) < 0) {
    DBG_MSG("play_wav_thread_t Resume error");
    return false;
  } else {
    return true;
  }
}

bool audio_play_t::stop()
{
  if (m_audio_driver->Stop(&m_wm8994, WM8994_PDWN_SW) < 0) {
    DBG_MSG("play_wav_thread_t Stop error");
    return false;
  } else {
    stop_sending_data();
    return true;
  }
}

bool audio_play_t::start_sending_data(uint8_t* a_data, size_t a_data_size)
{
  if(HAL_SAI_Transmit_DMA(&m_sai_handle, a_data, a_data_size) != HAL_OK) {
    DBG_MSG("play_wav_thread_t dma transmit error");
    return false;
  } else {
    return true;
  }
}

bool audio_play_t::pause_sending_data()
{
  if(HAL_SAI_DMAPause(&m_sai_handle) != HAL_OK) {
    DBG_MSG("play_wav_thread_t dma pause error");
    return false;
  } else {
    return true;
  }
}

bool audio_play_t::resume_sending_data()
{
  if(HAL_SAI_DMAResume(&m_sai_handle) != HAL_OK) {
    DBG_MSG("play_wav_thread_t dma resume error");
    return false;
  } else {
    return true;
  }
}

bool audio_play_t::stop_sending_data()
{
  if(HAL_SAI_DMAStop(&m_sai_handle) != HAL_OK) {
    DBG_MSG("play_wav_thread_t dma stop error");
    return false;
  } else {
    return true;
  }
}

void audio_play_t::set_volume(int a_volume)
{
  WM8994_SetVolume(&m_wm8994, VOLUME_OUTPUT, a_volume);
}

void audio_play_t::sai_init()
{
  // Ноги
  GPIO_InitTypeDef  sai_gpio{0};
  sai_gpio.Mode = GPIO_MODE_AF_PP;
  sai_gpio.Pull = GPIO_NOPULL;
  sai_gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  sai_gpio.Alternate = GPIO_AF6_SAI1;
  sai_gpio.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOE, &sai_gpio);

  sai_gpio.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOE, &sai_gpio);

  sai_gpio.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOE, &sai_gpio);

  sai_gpio.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOE, &sai_gpio);

  // DMA
  __HAL_RCC_DMA2_CLK_ENABLE();

  m_dma_sai_handle.Instance = DMA2_Stream6;
  m_dma_sai_handle.Init.Request = DMA_REQUEST_SAI1_A;
  m_dma_sai_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
  m_dma_sai_handle.Init.PeriphInc = DMA_PINC_DISABLE;
  m_dma_sai_handle.Init.MemInc = DMA_MINC_ENABLE;
  m_dma_sai_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  m_dma_sai_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  m_dma_sai_handle.Init.Mode = DMA_CIRCULAR;
  m_dma_sai_handle.Init.Priority = DMA_PRIORITY_HIGH;
  m_dma_sai_handle.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  m_dma_sai_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  m_dma_sai_handle.Init.MemBurst = DMA_MBURST_SINGLE;
  m_dma_sai_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;

  __HAL_LINKDMA(&m_sai_handle, hdmatx, m_dma_sai_handle);

  HAL_DMA_DeInit(&m_dma_sai_handle);
  if (HAL_DMA_Init(&m_dma_sai_handle) != HAL_OK) {
    DBG_MSG("DMA SAI init error!");
  }

  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

  //SAI
  __HAL_RCC_SAI1_CLK_ENABLE();

  m_sai_handle.Instance = SAI1_Block_A;
  m_sai_handle.Init.AudioMode = SAI_MODEMASTER_TX;
  m_sai_handle.Init.Synchro = SAI_ASYNCHRONOUS;
  m_sai_handle.Init.OutputDrive = SAI_OUTPUTDRIVE_ENABLE;
  m_sai_handle.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  m_sai_handle.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_1QF;
  m_sai_handle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_44K;
  m_sai_handle.Init.Protocol = SAI_FREE_PROTOCOL;
  m_sai_handle.Init.DataSize = SAI_DATASIZE_16;
  m_sai_handle.Init.FirstBit = SAI_FIRSTBIT_MSB;
  m_sai_handle.Init.ClockStrobing = SAI_CLOCKSTROBING_FALLINGEDGE;

  m_sai_handle.FrameInit.FrameLength = 32;
  m_sai_handle.FrameInit.ActiveFrameLength = 16;
  m_sai_handle.FrameInit.FSDefinition = SAI_FS_CHANNEL_IDENTIFICATION;
  m_sai_handle.FrameInit.FSPolarity = SAI_FS_ACTIVE_LOW;
  m_sai_handle.FrameInit.FSOffset = SAI_FS_BEFOREFIRSTBIT;

  m_sai_handle.SlotInit.FirstBitOffset = 0;
  m_sai_handle.SlotInit.SlotSize = SAI_SLOTSIZE_DATASIZE;
  m_sai_handle.SlotInit.SlotNumber = 2;
  m_sai_handle.SlotInit.SlotActive = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  __HAL_SAI_DISABLE(&m_sai_handle);

  if(HAL_OK != HAL_SAI_Init(&m_sai_handle)) {
    DBG_MSG("SAI init error!");
  }

  __HAL_SAI_ENABLE(&m_sai_handle);
}

void audio_play_t::audio_codec_init()
{
  WM8994_IO_t wm_8994_io{0};
  wm_8994_io.Address = AUDIO_I2C_ADDRESS;
  wm_8994_io.Init = BSP_I2C1_Init;
  wm_8994_io.DeInit = BSP_I2C1_DeInit;
  wm_8994_io.ReadReg = BSP_I2C1_ReadReg16;
  wm_8994_io.WriteReg = BSP_I2C1_WriteReg16;
  wm_8994_io.GetTick = BSP_GetTick;

  int32_t ret = BSP_ERROR_NONE;
  if(WM8994_RegisterBusIO (&m_wm8994, &wm_8994_io) != WM8994_OK) {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else {
    uint32_t id;
    if(WM8994_Reset(&m_wm8994) != WM8994_OK) {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    } else if(WM8994_ReadID(&m_wm8994, &id) != WM8994_OK) {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    } else if(id != WM8994_ID) {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    } else {
      m_audio_driver = reinterpret_cast<AUDIO_Drv_t*>(&WM8994_Driver);
    }
  }

  if (ret != BSP_ERROR_NONE) {
    DBG_MSG("Audio codec init error!");
  }

  m_wm8994_init.Resolution  = 0;
  // По каким то причинам sr=44100 не работает в wm9884 (в примере st работает)
  m_wm8994_init.Frequency  = 44100;//SAI_AUDIO_FREQUENCY_22K;
  m_wm8994_init.InputDevice = WM8994_IN_NONE;
  m_wm8994_init.OutputDevice = AUDIO_OUT_DEVICE_HEADPHONE;

  /* Convert volume before sending to the codec */
  m_wm8994_init.Volume = VOLUME_OUT_CONVERT(35);

  if(m_audio_driver->Init(&m_wm8994, &m_wm8994_init) != 0) {
//    DBG_MSG("Audio driver init error!");
  }
}

DMA_HandleTypeDef* audio_play_t::dma_sai_handle()
{
   return &m_dma_sai_handle;
}

