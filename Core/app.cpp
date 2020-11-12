#include "app.h"
#include "cmsis_os.h"
#include "portable.h"
#include "thread.h"

app_t::app_t() :
  mp_cfg(&cfg_t::instance()),
  touchgfx_thread()
{
  create_thread(touchgfx_thread, "touchgfx_thread", osPriorityNormal, 2048);

  vTaskStartScheduler();
}