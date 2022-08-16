#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, TickPriority ,0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  __HAL_RCC_TIM6_CLK_ENABLE();

  uint32_t timer_clock = 2 * HAL_RCC_GetPCLK1Freq();

  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  TIM_HandleTypeDef htim6 = { 0 };
  htim6.Instance = TIM6;
  htim6.Init.Period = (1'000'000 / 1000) - 1;
  htim6.Init.Prescaler = (timer_clock / 1'000'000) - 1;
  htim6.Init.ClockDivision = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&htim6) == HAL_OK) {
    return HAL_TIM_Base_Start_IT(&htim6);
  } else {
    return HAL_ERROR;
  }
}
