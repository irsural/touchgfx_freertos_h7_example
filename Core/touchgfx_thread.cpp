#include "touchgfx_thread.h"

touchgfx_thread_t::touchgfx_thread_t() :
  m_tc(),
  m_dma(),
  m_display(),
  m_font_provider(),
  m_texts(),
  m_hal(m_dma, m_display, m_tc, 640, 480)
{
  Bitmap::registerBitmapDatabase(BitmapDatabase::getInstance(), BitmapDatabase::getInstanceSize());
  TypedText::registerTexts(&m_texts);
  Texts::setLanguage(0);

  FontManager::setFontProvider(&m_font_provider);

  FrontendHeap& heap = FrontendHeap::getInstance();
  (void)heap; // we need to obtain the reference above to initialize the frontend heap.

  m_hal.setFrameRateCompensation(true);
  m_hal.lockDMAToFrontPorch(false);

  m_hal.initialize();
}

void touchgfx_thread_t::task()
{
  FrontendHeap::getInstance().model.set_gui_thread(this);
  m_hal.taskEntry();
}
