#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <string>
#include <ff.h>

namespace fatfs
{

namespace sd
{
  inline const char* drive = "0:/";
  inline FATFS* fs;

  bool init_fs();
}

class dir
{
public:
  dir(const char* a_path);
  ~dir();

  std::string get_next();

private:
  DIR m_dir;
  FILINFO m_f_info;
};


class file_t
{
public:
  enum class result_t {
    success,
    eof,
    error,
  };

  file_t(const char* a_path, uint32_t a_access_mode);
  ~file_t();

  result_t read(uint8_t* a_buffer, uint32_t a_buffer_size);
  FRESULT get_error();
private:
  FIL m_file;
  FRESULT m_error;
};


}

#endif // FILE_OPERATIONS_H
