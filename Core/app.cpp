#include "app.h"
#include "cmsis_os.h"


extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

app_t::app_t() :
  mp_cfg(&cfg_t::instance())
{
  touchgfx_init();
  
  osKernelInitialize();
  
  const osThreadAttr_t TouchGFXTask_attributes = {
    .name = "TouchGFXTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 2048
  };
  
  osThreadId_t TouchGFXTaskHandle = osThreadNew([](void *a_argument) { touchgfx_taskEntry(); }, 
    NULL, &TouchGFXTask_attributes);
  
  osKernelStart();
}