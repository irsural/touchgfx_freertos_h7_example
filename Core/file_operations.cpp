#include "file_operations.h"
#include "sd_diskio_dma_rtos.h"
#include "utils.h"


bool fatfs::sd::init_fs()
{
  bool res = false;
  fs = mem::aligined_alloc<FATFS>(32);

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


fatfs::file_t::file_t(const char* a_path, uint32_t a_access_mode) :
  m_file({0}),
  m_filename(a_path),
  m_error(FR_OK)
{
  m_error = f_open(&m_file, a_path, a_access_mode);
}

std::string fatfs::file_t::name()
{
  return m_filename;
}

FRESULT fatfs::file_t::get_error()
{
  return m_error;
}

fatfs::file_t::~file_t()
{
  f_close(&m_file);
}

fatfs::file_t::result_t fatfs::file_t::read(uint8_t* a_buffer, uint32_t a_buffer_size)
{
  uint32_t bytes_read = 0;
  m_error = f_read(&m_file, a_buffer, a_buffer_size, &bytes_read);

  result_t read_result = result_t::error;
  if (m_error == FR_OK && bytes_read == 0) {
    read_result = result_t::eof;
  } else if (m_error == FR_OK) {
    read_result = result_t::success;
  }
  return read_result;
}

void fatfs::file_t::seek(size_t a_byte_offset)
{
  m_error = f_lseek(&m_file, a_byte_offset);
}













