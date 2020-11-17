#ifndef READ_WAV_FILES_THREAD_H
#define READ_WAV_FILES_THREAD_H

#include "thread.h"
#include "file_operations.h"

#include <vector>
#include <array>

#include <touchgfx/Unicode.hpp>


class read_dir_thread_t : public thread_base_t
{
public:
  using char_type = touchgfx::Unicode::UnicodeChar;

  read_dir_thread_t(QueueHandle_t a_target_folder_queue, const char* a_ext, thread_base_t* a_parent_thread);
  ~read_dir_thread_t();
  virtual void task() override;

  const std::vector<std::vector<char_type>>& get_names() const;
  void clear_names();

private:
  QueueHandle_t m_target_folder_queue;
  char* m_current_folder_path;

  std::vector<std::vector<char_type>> dir_names;
  std::string m_extension;

  thread_base_t* m_parent_thread;
};


#endif // READ_WAV_FILES_THREAD_H
