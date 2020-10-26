#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 
   
   
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM6_DAC_IRQHandler(void);
void LTDC_IRQHandler(void);
void DMA2D_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */
