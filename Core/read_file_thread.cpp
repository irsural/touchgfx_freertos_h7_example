#include "read_file_thread.h"
#include "codecvt_oem866.h"
#include "cfg.h"


read_file_thread_t::read_file_thread_t(const char* a_filename, std::vector<uint8_t> a_read_buffer) :
  m_file(a_filename, FA_READ)
{

}

read_file_thread_t::~read_file_thread_t()
{
  vTaskDelete(m_handle);
}

void read_file_thread_t::task()
{
//  while(1) {
//    if (xQueueReceive(m_target_folder_queue, static_cast<void*>(&m_current_folder_path), portMAX_DELAY) == pdTRUE) {
//      assert(dir_names.empty());
//
//      if (cfg_t::instance().is_sd_detected()) {
//        fatfs::dir root_dir(m_current_folder_path);
//
//        std::string file_name = root_dir.get_next();
//        while (file_name != "") {
//          if (ends_with(file_name, m_extension)) {
//            DBG_MSG(file_name);
//            dir_names.push_back(oem866_to_unicode(file_name.c_str()));
//          }
//          file_name = root_dir.get_next();
//        }
//      }
//      xTaskNotifyGive(m_parent_thread->m_handle);
//    }
//  }
}