/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ItemsListContainerBase.hpp>
#include "BitmapDatabase.hpp"
#include <touchgfx/Color.hpp>

ItemsListContainerBase::ItemsListContainerBase() :
    ItemChoosenCallback(0)
{
    setWidth(180);
    setHeight(480);
    tiledImage1.setBitmap(touchgfx::Bitmap(BITMAP_BLUE_TEXTURES_GPLAYPATTERN_ID));
    tiledImage1.setPosition(0, 0, 180, 509);
    tiledImage1.setAlpha(49);
    tiledImage1.setOffset(0, 0);

    scroll_area.setPosition(0, 0, 180, 509);
    scroll_area.enableHorizontalScroll(false);
    scroll_area.setScrollbarsColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    list_layout.setDirection(touchgfx::SOUTH);
    list_layout.setPosition(0, 0, 200, 29);
    scroll_area.add(list_layout);

    add(tiledImage1);
    add(scroll_area);
}

void ItemsListContainerBase::initialize()
{

}
