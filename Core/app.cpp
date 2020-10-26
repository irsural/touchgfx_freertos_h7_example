#include "app.h"
#include <vector>
#include <numeric>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/OSWrappers.hpp>


extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

app_t::app_t() :
  mp_cfg(&cfg_t::instance())
{
  touchgfx_init();
  
  auto hal = touchgfx::HAL::getInstance();
  
  hal->enableLCDControllerInterrupt();
  hal->enableInterrupts();

  touchgfx::OSWrappers::waitForVSync();
  hal->backPorchExited();

#if defined(LCD_RESET_GPIO_Port) && defined(LCD_RESET_Pin)
  /* Display Enable */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port , LCD_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_Port , LCD_RESET_Pin, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */
#endif
  
#if defined(LCD_DE_GPIO_Port) && defined(LCD_DE_Pin)
  /* Data Enable */
  HAL_GPIO_TogglePin(LCD_DE_GPIO_Port, LCD_DE_Pin);
#endif
  
#if defined(LCD_BL_CTRL_GPIO_Port) && defined(LCD_BL_CTRL_Pin)
  /* Backlight Enable */
  HAL_GPIO_TogglePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin);
#endif
}

void app_t::tick()
{
  std::vector<double> av;
  av.reserve(20);
  
  while(1) {
    
    if (av.size() == 20) {
      
      double res = std::accumulate(av.begin(), av.end(), 0) / av.size();
      
      static char buf[20];
      snprintf(buf, sizeof(buf), "Gui: %f", res);
      
      touchgfx::Application::getDebugPrinter()->setString(buf);
      touchgfx::Application::invalidateDebugRegion();
      
      av.clear();
    }
    
    if (touchgfx::OSWrappers::isVSyncAvailable()) {
      
      uint32_t time1 = HAL_GetTick();
      
      touchgfx::HAL::getInstance()->backPorchExited();
      touchgfx::OSWrappers::signalRenderingDone();
      
      uint32_t time2 = HAL_GetTick();
      
      double gui_time = time2 - time1;
      
      if (gui_time) {
        av.push_back(gui_time);
      }
    }
  }
}
