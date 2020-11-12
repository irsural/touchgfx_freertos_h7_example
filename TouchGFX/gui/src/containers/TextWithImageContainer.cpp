#include <gui/containers/TextWithImageContainer.hpp>
#include <algorithm>
#include "codecvt_oem866.h"

TextWithImageContainer::TextWithImageContainer() :
  m_wildcard_buffer()
{
  setTouchable(true);
}

TextWithImageContainer::TextWithImageContainer(std::vector<touchgfx::Unicode::UnicodeChar> a_text)
{
  setTouchable(true);
  setText(std::move(a_text));
}

void TextWithImageContainer::initialize()
{
    TextWithImageContainerBase::initialize();
}

void TextWithImageContainer::setText(std::vector<touchgfx::Unicode::UnicodeChar> a_text)
{
  // Этот файл должен быть в кодировке UTF-8 !!
  // Unicode::fromUTF8((const uint8_t*)"Какой то текст", m_wildcard_buffer, TEXTAREA_SIZE);

  m_wildcard_buffer = std::move(a_text);
  _text.setWildcard(m_wildcard_buffer.data());
  _text.invalidate();
}

void TextWithImageContainer::setImage(const Bitmap& a_bmp)
{
  _image.setBitmap(a_bmp);
}

void TextWithImageContainer::set_item_clicked_callback(touchgfx::GenericCallback<TextWithImageContainer*>& a_callback)
{
  m_item_clicked_callback = &a_callback;
}

void TextWithImageContainer::handleClickEvent(const ClickEvent &event)
{
  static bool clickCancelled = false;
  //do not select movie on release, if click was cancelled due to drag.
  switch (event.getType())
  {
  case ClickEvent::PRESSED:
      break;
  case ClickEvent::RELEASED:
      if (!clickCancelled)
      {
        _image.setBitmap(Bitmap());

        if (m_item_clicked_callback && m_item_clicked_callback->isValid()) {
          m_item_clicked_callback->execute(this);
        }
      }
      clickCancelled = false;
      break;
  case ClickEvent::CANCEL:
      clickCancelled = true;
      break;
  }
}
