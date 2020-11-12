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


}

#endif // FILE_OPERATIONS_H
