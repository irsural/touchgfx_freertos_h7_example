#include <gui/main_screen/MainView.hpp>
#include <touchgfx/Color.hpp>

MainView::MainView()
{
}

void MainView::setupScreen()
{
}

void MainView::tearDownScreen()
{
}

void MainView::set_random_color()
{
  backgroundBox.setColor(touchgfx::Color::getColorFrom24BitRGB(rand()&0xff, rand()&0xff, rand()&0xff));
  backgroundBox.invalidate();
}

void MainView::start_stop_animation()
{
}
