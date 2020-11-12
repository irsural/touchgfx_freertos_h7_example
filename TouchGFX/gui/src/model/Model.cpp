#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/Application.hpp>

#include <stm32h7xx_hal.h>
#include <stdio.h>

Model::Model() :
  modelListener(nullptr),
  m_debug_str("")
{
}

void Model::tick()
{
  show_fps();
}

void Model::show_fps()
{
  static uint32_t prev_time = HAL_GetTick();
  static uint32_t current_frames = 0;
  static uint32_t count = 0;

  if (HAL_GetTick() - prev_time >= 1000) {
    m_debug_str = "FPS(" + std::to_string(count) + "): " + std::to_string(current_frames);
    touchgfx::Application::getDebugPrinter()->setString(m_debug_str.c_str());
    touchgfx::Application::invalidateDebugRegion();

    current_frames = 0;
    prev_time = HAL_GetTick();
    count++;
  } else {
    current_frames++;
  }
}
