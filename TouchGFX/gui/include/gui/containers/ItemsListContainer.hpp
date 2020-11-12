#ifndef ITEMSLISTCONTAINER_HPP
#define ITEMSLISTCONTAINER_HPP

#include <gui_generated/containers/ItemsListContainerBase.hpp>
#include <gui/containers/TextWithImageContainer.hpp>
#include <vector>
#include <memory>

class ItemsListContainer : public ItemsListContainerBase
{
public:
  ItemsListContainer();
  virtual ~ItemsListContainer() {}

  virtual void initialize();

  void add_item(std::vector<touchgfx::Unicode::UnicodeChar> a_item_text);
  size_t items_count();

  void set_item_image(uint16_t a_bitmap_id);
private:
  std::vector<std::shared_ptr<TextWithImageContainer>> m_items;
  Callback<ItemsListContainer, TextWithImageContainer*> m_update_pressed_item_callback;
  size_t m_last_pressed_item;

  void update_pressed_item(TextWithImageContainer *a_item_addr);
};

#endif // ITEMSLISTCONTAINER_HPP
