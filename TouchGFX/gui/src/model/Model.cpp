#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/hal/HAL.hpp>

#include <stm32h7xx_hal.h>
#include <stdio.h>

Model::Model() :
  modelListener(nullptr),
  m_debug_str(""),
  m_debug_tick(0),
  m_gui_thread(nullptr)
{
}

void Model::tick()
{
  show_mcu_load();

  modelListener->presenter_tick();
}

void Model::show_mcu_load()
{
  #if TEMPLATE_DEBUG
  if (HAL_GetTick() - m_debug_tick > 1000) {
    m_debug_tick = HAL_GetTick();
    uint32_t mcu_load = touchgfx::HAL::getInstance()->getMCULoadPct();
    m_debug_str = "MCU Load(" + std::to_string(m_debug_tick / 1000) + "): " + std::to_string(mcu_load);
    touchgfx::Application::getDebugPrinter()->setString(m_debug_str.c_str());
    touchgfx::Application::invalidateDebugRegion();
  }
  #endif // TEMPLATE_DEBUG
}

void Model::set_gui_thread(thread_base_t* a_thread)
{
  m_gui_thread = a_thread;
}

thread_base_t* Model::get_gui_thread()
{
  return m_gui_thread;
}
