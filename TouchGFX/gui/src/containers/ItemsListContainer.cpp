#include <gui/containers/ItemsListContainer.hpp>
#include <BitmapDatabase.hpp>
#include <algorithm>

ItemsListContainer::ItemsListContainer() :
  m_items(),
  m_update_pressed_item_callback(this, &ItemsListContainer::update_pressed_item)
{

}

void ItemsListContainer::initialize()
{
  ItemsListContainerBase::initialize();
}

void ItemsListContainer::add_item(std::vector<touchgfx::Unicode::UnicodeChar> a_item_text)
{
  m_items.push_back(std::make_shared<TextWithImageContainer>(std::move(a_item_text)));

  m_items.back()->set_item_clicked_callback(m_update_pressed_item_callback);
  list_layout.add(*m_items.back());
  list_layout.invalidate();
}

void ItemsListContainer::update_pressed_item(TextWithImageContainer *a_item_addr)
{
  size_t pressed_item_idx = 9999;

  for (size_t i = 0; i < m_items.size(); ++i) {
    if (m_items[i].get() == a_item_addr) {
      pressed_item_idx = i;
      break;
    }
  }
  assert(pressed_item_idx != 9999);

  m_items[m_last_pressed_item]->setImage(Bitmap());
  m_items[pressed_item_idx]->setImage(Bitmap(BITMAP_PLAY_IMAGE_ID));
  m_last_pressed_item = pressed_item_idx;

  list_layout.invalidate();
}
