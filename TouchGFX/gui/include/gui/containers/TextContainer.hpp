#ifndef TEXTCONTAINER_HPP
#define TEXTCONTAINER_HPP

#include <gui_generated/containers/TextContainerBase.hpp>

#include <vector>

class TextContainer : public TextContainerBase
{
public:
  TextContainer();
  TextContainer(const char* a_text);
  virtual ~TextContainer() {}
  
  virtual void initialize();
  
  void setText(const char* a_text);
  
private:
  std::vector<touchgfx::Unicode::UnicodeChar> m_wildcard_buffer;
};

#endif // TEXTCONTAINER_HPP
