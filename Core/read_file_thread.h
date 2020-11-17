#ifndef READ_FILE_THREAD_H
#define READ_FILE_THREAD_H

#include "thread.h"
#include "file_operations.h"

#include <vector>
#include <array>

#include <touchgfx/Unicode.hpp>


class read_file_thread_t : public thread_base_t
{
public:
  using char_type = touchgfx::Unicode::UnicodeChar;

  read_file_thread_t(const char* a_filename, std::vector<uint8_t> a_read_buffer);
  ~read_file_thread_t();
  virtual void task() override;

private:
  fatfs::file_t m_file;
};


#endif // READ_FILE_THREAD_H
