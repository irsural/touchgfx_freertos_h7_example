#include "file_operations.h"
#include "sd_diskio_dma_rtos.h"
#include <cassert>

template <class T>
T* aligined_alloc(size_t a_align)
{
  void* p_mem = ::operator new(sizeof(T) + a_align);
  size_t aligned_address = (reinterpret_cast<size_t>(p_mem) & ~(a_align - 1)) + a_align;

  assert(aligned_address % a_align == 0);
  assert(aligned_address + sizeof(T) <= reinterpret_cast<size_t>(p_mem) + sizeof(T) + a_align);

  return reinterpret_cast<T*>(aligned_address);
}

bool fatfs::sd::init_fs()
{
  bool res = false;
  fs = aligined_alloc<FATFS>(32);

  if(FATFS_LinkDriver(&SD_Driver, (TCHAR*)drive) == 0) {
    if (!f_mount(fs, drive, 0)) {
      res = true;
    }
  }
  return res;
}

fatfs::dir::dir(const char* a_path)
{
  FRESULT res = f_opendir(&m_dir, a_path);
  assert(res == FR_OK);
}

fatfs::dir::~dir()
{
  f_closedir(&m_dir);
}

std::string fatfs::dir::get_next()
{
  FRESULT res = f_readdir(&m_dir, &m_f_info);
  assert(res == FR_OK);

  if (m_f_info.fname[0] == 0)
    return "";
  if (!(m_f_info.fattrib & AM_DIR)) {
    return m_f_info.fname;
  } else {
    return get_next();
  }
}
