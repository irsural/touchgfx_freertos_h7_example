#ifndef TOUCHGFX_THREAD_H
#define TOUCHGFX_THREAD_H

#include "thread.h"
#include <texts/TypedTextDatabase.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <BitmapDatabase.hpp>
#include <platform/driver/lcd/LCD16bpp.hpp>
#include <STM32DMA.hpp>
#include <TouchGFXHAL.hpp>
#include <STM32TouchController.hpp>
#include <stm32h7xx_hal.h>

class touchgfx_thread_t : public thread_base_t
{
public:
  touchgfx_thread_t();
  virtual void task() override;
private:
   STM32TouchController m_tc;
   STM32H7DMA m_dma;
   LCD16bpp m_display;
   ApplicationFontProvider m_font_provider;
   Texts m_texts;
   TouchGFXHAL m_hal;
};



#endif // TOUCHGFX_THREAD_H
