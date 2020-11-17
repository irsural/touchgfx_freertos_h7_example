#include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap) :
  FrontendApplicationBase(m, heap),
  m_display_logger()
{
  m_display_logger.setPosition(10, 10, 240, 40);
  m_display_logger.setScale(2);
  m_display_logger.setColor(0xFF00); //black
  Application::setDebugPrinter(&m_display_logger);
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    MVPApplication::handleClickEvent(evt);
}