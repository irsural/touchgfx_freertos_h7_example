#include "app_touchgfx.h"

void touchgfx_init(void);
void touchgfx_taskEntry(void);

/**
 * Initialize TouchGFX application
 */
void MX_TouchGFX_Init(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_init();
}

/**
 * TouchGFX application entry function
 */
void MX_TouchGFX_Process(void)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

/**
 * TouchGFX application thread
 */
void TouchGFX_Task(void *argument)
{
  // Calling farward to touchgfx_init in C++ domain
  touchgfx_taskEntry();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
