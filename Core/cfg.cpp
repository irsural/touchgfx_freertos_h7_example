#include "cfg.h"

#include <stm32h743i_eval_sdram.h>
#include <stm32h743i_eval_qspi.h>

cfg_t::cfg_t() :
  m_lcd_vsync_pin_port(std::make_tuple(GPIO_PIN_10, GPIOF)),
  m_lcd_reset_pin_port(std::make_tuple(GPIO_PIN_2, GPIOF)),
  m_lcd_bl_ctrl_pin_port(std::make_tuple(GPIO_PIN_6, GPIOF)),
  m_lcd_render_time_pin_port(std::make_tuple(GPIO_PIN_4, GPIOF)),
  m_ltd_handle{ 0 },
  m_dma2d_handle{ 0 },
  m_qspi_handle{ 0 }
{
  gpio_init();
  crc_init();
  ltdc_init();
  fmc_init();
  dma2d_init();
  qspi_init();
}

void cfg_t::gpio_init()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
}

void cfg_t::crc_init()
{
  CRC_HandleTypeDef hcrc = { 0 };
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  
  __HAL_RCC_CRC_CLK_ENABLE();
  
  if (HAL_CRC_Init(&hcrc) != HAL_OK) {
    DBG_MSG("CRC init error!!");
  }
}

void cfg_t::ltdc_init()
{
  GPIO_InitTypeDef gpio = { 0 };

  HAL_GPIO_WritePin(std::get<1>(m_lcd_vsync_pin_port), std::get<0>(m_lcd_vsync_pin_port), GPIO_PIN_SET);
  HAL_GPIO_WritePin(std::get<1>(m_lcd_reset_pin_port), std::get<0>(m_lcd_reset_pin_port), GPIO_PIN_RESET);
  HAL_GPIO_WritePin(std::get<1>(m_lcd_bl_ctrl_pin_port), std::get<0>(m_lcd_bl_ctrl_pin_port), GPIO_PIN_SET);
  HAL_GPIO_WritePin(std::get<1>(m_lcd_render_time_pin_port), std::get<0>(m_lcd_render_time_pin_port), GPIO_PIN_SET);

  gpio.Pin = std::get<0>(m_lcd_vsync_pin_port);
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(std::get<1>(m_lcd_vsync_pin_port), &gpio);

  gpio.Pin = std::get<0>(m_lcd_reset_pin_port);
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(std::get<1>(m_lcd_reset_pin_port), &gpio);

  gpio.Pin = std::get<0>(m_lcd_bl_ctrl_pin_port);
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(std::get<1>(m_lcd_bl_ctrl_pin_port), &gpio);

  gpio.Pin = std::get<0>(m_lcd_render_time_pin_port);
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_PULLUP;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(std::get<1>(m_lcd_render_time_pin_port), &gpio);
  
  
  m_ltd_handle.Instance = LTDC;
  m_ltd_handle.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  m_ltd_handle.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  m_ltd_handle.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  m_ltd_handle.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  m_ltd_handle.Init.HorizontalSync = 29;
  m_ltd_handle.Init.VerticalSync = 2;
  m_ltd_handle.Init.AccumulatedHBP = 143;
  m_ltd_handle.Init.AccumulatedVBP = 34;
  m_ltd_handle.Init.AccumulatedActiveW = 783;
  m_ltd_handle.Init.AccumulatedActiveH = 514;
  m_ltd_handle.Init.TotalWidth = 799;
  m_ltd_handle.Init.TotalHeigh = 524;
  m_ltd_handle.Init.Backcolor.Blue = 0;
  m_ltd_handle.Init.Backcolor.Green = 0;
  m_ltd_handle.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&m_ltd_handle) != HAL_OK) {
    DBG_MSG("LTDC init error")
  }
  
  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 640;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = 640;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&m_ltd_handle, &pLayerCfg, 0) != HAL_OK) {
    DBG_MSG("LTDC Layer init error")
  }
}

void cfg_t::fmc_init()
{
  SDRAM_HandleTypeDef sdram_handle = { 0 };
  
  sdram_handle.Instance = FMC_SDRAM_DEVICE;
  sdram_handle.Init.SDBank = FMC_SDRAM_BANK2;
  sdram_handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  sdram_handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_11;
  sdram_handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  sdram_handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_2;
  sdram_handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_1;
  sdram_handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  sdram_handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_DISABLE;
  sdram_handle.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  sdram_handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  
  FMC_SDRAM_TimingTypeDef sdram_timings = {0};
  sdram_timings.LoadToActiveDelay = 16;
  sdram_timings.ExitSelfRefreshDelay = 16;
  sdram_timings.SelfRefreshTime = 16;
  sdram_timings.RowCycleDelay = 16;
  sdram_timings.WriteRecoveryTime = 16;
  sdram_timings.RPDelay = 16;
  sdram_timings.RCDDelay = 16;

  if (HAL_SDRAM_Init(&sdram_handle, &sdram_timings) != HAL_OK) {
    DBG_MSG("LTDC Layer init error")
  }

  FMC_Bank1_R->BTCR[0] &= ~FMC_BCRx_MBKEN;

  BSP_SDRAM_DeInit(0);
  BSP_SDRAM_Init(0);
}

void cfg_t::dma2d_init()
{
  m_dma2d_handle.Instance = DMA2D;
  m_dma2d_handle.Init.Mode = DMA2D_R2M;
  m_dma2d_handle.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  m_dma2d_handle.Init.OutputOffset = 0;
  m_dma2d_handle.Init.BytesSwap = DMA2D_BYTES_REGULAR;
  m_dma2d_handle.Init.LineOffsetMode = DMA2D_LOM_PIXELS;
  m_dma2d_handle.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
  if (HAL_DMA2D_Init(&m_dma2d_handle) != HAL_OK) {
    DBG_MSG("DMA2D init error")
  }
}

void cfg_t::qspi_init()
{
  m_qspi_handle.Instance = QUADSPI;
  m_qspi_handle.Init.ClockPrescaler = 3;
  m_qspi_handle.Init.FifoThreshold = 1;
  m_qspi_handle.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  m_qspi_handle.Init.FlashSize = 26;
  m_qspi_handle.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
  m_qspi_handle.Init.ClockMode = QSPI_CLOCK_MODE_0;
  m_qspi_handle.Init.DualFlash = QSPI_DUALFLASH_ENABLE;
  
  if (HAL_QSPI_Init(&m_qspi_handle) != HAL_OK) {
    DBG_MSG("QSPI init error 1")
  }
  BSP_QSPI_DeInit (0);

  BSP_QSPI_Init_t qspi_init;
  qspi_init.InterfaceMode  = MT25TL01G_QPI_MODE;
  qspi_init.TransferRate   = MT25TL01G_DTR_TRANSFER;
  qspi_init.DualFlashMode  = MT25TL01G_DUALFLASH_ENABLE;
  
  if(BSP_QSPI_Init(0,&qspi_init) != BSP_ERROR_NONE) {
    DBG_MSG("QSPI init error 2")
  }
  if(BSP_QSPI_EnableMemoryMappedMode(0) != BSP_ERROR_NONE) {
    DBG_MSG("QSPI init error 3")
  }
  HAL_NVIC_DisableIRQ(QUADSPI_IRQn);
}

LTDC_HandleTypeDef *cfg_t::ltdc_handle()
{
  return &m_ltd_handle;
}

DMA2D_HandleTypeDef *cfg_t::dma2d_handle()
{
  return &m_dma2d_handle;
}
