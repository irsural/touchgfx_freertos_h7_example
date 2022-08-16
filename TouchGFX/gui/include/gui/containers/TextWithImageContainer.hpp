#ifndef TEXTWITHIMAGECONTAINER_HPP
#define TEXTWITHIMAGECONTAINER_HPP

#include <gui_generated/containers/TextWithImageContainerBase.hpp>
#include <vector>

class TextWithImageContainer : public TextWithImageContainerBase
{
public:
  TextWithImageContainer();
  TextWithImageContainer(std::vector<touchgfx::Unicode::UnicodeChar> a_text);
  virtual ~TextWithImageContainer() {}

  virtual void initialize();

  void setText(std::vector<touchgfx::Unicode::UnicodeChar> a_text);
  void setImage(const Bitmap& a_bmp);

  const std::vector<touchgfx::Unicode::UnicodeChar>& text() const noexcept;

  virtual void handleClickEvent(const ClickEvent &event);

  void set_item_clicked_callback(touchgfx::GenericCallback<TextWithImageContainer*>& a_callback);
private:
  std::vector<touchgfx::Unicode::UnicodeChar> m_wildcard_buffer;
  GenericCallback<TextWithImageContainer*> *m_item_clicked_callback;
};

#endif // TEXTWITHIMAGECONTAINER_HPP
