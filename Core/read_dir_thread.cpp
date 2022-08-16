#include "read_dir_thread.h"
#include "codecvt_oem866.h"
#include "cfg.h"


bool ends_with(std::string const &a_string, std::string const &a_ending) {
  if (a_string.length() >= a_ending.length()) {
    return (a_string.compare(a_string.length() - a_ending.length(), a_ending.length(), a_ending) == 0);
  } else {
    return false;
  }
}

read_dir_thread_t::read_dir_thread_t(QueueHandle_t a_target_folder_queue, const char* a_ext,
  thread_base_t* a_parent_thread) :
  m_target_folder_queue(a_target_folder_queue),
  m_current_folder_path(nullptr),
  dir_names(),
  m_extension(a_ext),
  m_parent_thread(a_parent_thread)
{

}

read_dir_thread_t::~read_dir_thread_t()
{
  vTaskDelete(m_handle);
}

void read_dir_thread_t::task()
{
  while(1) {
    if (xQueueReceive(m_target_folder_queue, static_cast<void*>(&m_current_folder_path), portMAX_DELAY) == pdTRUE) {
      assert(dir_names.empty());

      if (cfg_t::instance().is_sd_detected()) {
        fatfs::dir root_dir(m_current_folder_path);

        std::string file_name = root_dir.get_next();
        while (file_name != "") {
          if (ends_with(file_name, m_extension)) {
            DBG_MSG(file_name);
            dir_names.push_back(oem866_to_unicode(file_name.c_str()));
          }
          file_name = root_dir.get_next();
        }
      }
      xTaskNotifyGive(m_parent_thread->m_handle);
    }
  }
}

const std::vector<std::vector<read_dir_thread_t::char_type>>& read_dir_thread_t::get_names() const
{
  return dir_names;
}

void read_dir_thread_t::clear_names()
{
  dir_names.clear();
}
