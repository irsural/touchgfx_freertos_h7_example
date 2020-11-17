#include "stm32h7xx_it.h"
#include "FreeRTOS.h"
#include "task.h"

#include "cfg.h"


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1) { }
}

void MemManage_Handler(void)
{
  while (1) { }
}

void BusFault_Handler(void)
{
  while (1) { }
}

void UsageFault_Handler(void)
{
  while (1) { }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

void SDMMC1_IRQHandler(void)
{
  BSP_SD_IRQHandler(0);
}

void TIM6_DAC_IRQHandler(void)
{
  TIM6->SR = ~TIM_IT_UPDATE;
  HAL_IncTick();
}

void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(cfg_t::instance().ltdc_handle());
}

void DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(cfg_t::instance().dma2d_handle());
}
