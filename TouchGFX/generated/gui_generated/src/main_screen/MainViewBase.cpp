/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/main_screen/MainViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"

MainViewBase::MainViewBase() :
    buttonCallback(this, &MainViewBase::buttonCallbackHandler)
{

    __background.setPosition(0, 0, 640, 480);
    __background.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    backgroundBox.setPosition(0, 0, 800, 480);
    backgroundBox.setVisible(false);
    backgroundBox.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    bg3.setXY(544, 0);
    bg3.setBitmap(touchgfx::Bitmap(BITMAP_DARK_BACKGROUNDS_MAIN_BG_PORTRAIT_272X480PX_ID));

    bg2.setXY(272, 0);
    bg2.setBitmap(touchgfx::Bitmap(BITMAP_DARK_BACKGROUNDS_MAIN_BG_PORTRAIT_272X480PX_ID));

    bg1.setXY(0, 0);
    bg1.setBitmap(touchgfx::Bitmap(BITMAP_DARK_BACKGROUNDS_MAIN_BG_PORTRAIT_272X480PX_ID));

    openModal1.setXY(44, 311);
    openModal1.setBitmaps(touchgfx::Bitmap(BITMAP_SHOW_MODAL_BUTTON_ID), touchgfx::Bitmap(BITMAP_SHOW_MODAL_BUTTON_PRESSED_ID));
    openModal1.setAction(buttonCallback);

    openMyModal.setXY(479, 298);
    openMyModal.setBitmaps(touchgfx::Bitmap(BITMAP_SHOW_MY_MODAL_BUTTON_ID), touchgfx::Bitmap(BITMAP_SHOW_MY_MODAL_BUTTON_PRESSED_ID));
    openMyModal.setAction(buttonCallback);

    myModal.setBackground(touchgfx::BitmapId(BITMAP_MY_MODAL_BACKGROUND_ID), 70, 16);
    myModal.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    myModal.setShadeAlpha(150);
    myModal.hide();

    infoImage.setXY(120, 34);
    infoImage.setBitmap(touchgfx::Bitmap(BITMAP_INFO_ICON_ID));
    myModal.add(infoImage);

    closeMyModal.setXY(175, 166);
    closeMyModal.setBitmaps(touchgfx::Bitmap(BITMAP_CLOSE_BUTTON_ID), touchgfx::Bitmap(BITMAP_CLOSE_BUTTON_PRESSED_ID));
    closeMyModal.setAction(buttonCallback);
    myModal.add(closeMyModal);

    modal1.setBackground(touchgfx::BitmapId(BITMAP_MODAL_BACKGROUND_ID), 90, 36);
    modal1.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    modal1.setShadeAlpha(150);
    modal1.hide();

    closeModal1.setXY(155, 130);
    closeModal1.setBitmaps(touchgfx::Bitmap(BITMAP_CLOSE_BUTTON_ID), touchgfx::Bitmap(BITMAP_CLOSE_BUTTON_PRESSED_ID));
    closeModal1.setAction(buttonCallback);
    modal1.add(closeModal1);

    add(__background);
    add(backgroundBox);
    add(bg3);
    add(bg2);
    add(bg1);
    add(openModal1);
    add(openMyModal);
    add(myModal);
    add(modal1);
}

void MainViewBase::setupScreen()
{

}

void MainViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &openModal1)
    {
        //OpenModal1
        //When openModal1 clicked show modal1
        //Show modal1
        modal1.setVisible(true);
        modal1.invalidate();
    }
    else if (&src == &openMyModal)
    {
        //OpenMyModal
        //When openMyModal clicked show myModal
        //Show myModal
        myModal.setVisible(true);
        myModal.invalidate();
    }
    else if (&src == &closeMyModal)
    {
        //CloseMyModal
        //When closeMyModal clicked hide myModal
        //Hide myModal
        myModal.setVisible(false);
        myModal.invalidate();
    }
    else if (&src == &closeModal1)
    {
        //CloseModal1
        //When closeModal1 clicked hide modal1
        //Hide modal1
        modal1.setVisible(false);
        modal1.invalidate();
    }
}
