#include <gui/containers/TextContainer.hpp>
#include <algorithm>

TextContainer::TextContainer() :
  m_wildcard_buffer()
{

}

TextContainer::TextContainer(const char* a_text)
{
  setText(a_text);
}

void TextContainer::initialize()
{
}

void TextContainer::setText(const char *a_text)
{
  // Этот файл должен быть в кодировке UTF-8 !!
//  Unicode::fromUTF8((const uint8_t*)"Какой то текст", m_wildcard_buffer, TEXTAREA_SIZE);
  size_t size = std::max(strlen(a_text), 20u);
  
  m_wildcard_buffer.resize(size);
  Unicode::strncpy(m_wildcard_buffer.data(), a_text, size);
  
  _text.setWildcard(m_wildcard_buffer.data());
  _text.invalidate();
}
