#ifndef READ_FILE_THREAD_H
#define READ_FILE_THREAD_H

#include "thread.h"
#include "file_operations.h"

#include <vector>
#include <array>
#include <memory>

#include <touchgfx/Unicode.hpp>


class play_wav_thread_t : public thread_base_t
{
public:
  enum class cmd_t {
    none,
    play,
    pause,
    resume
  };

  using char_type = touchgfx::Unicode::UnicodeChar;

  play_wav_thread_t(QueueHandle_t a_cmd_queue);
  ~play_wav_thread_t();
  virtual void task() override;

  void set_wav_file(std::string a_filename);

  void play();
  void pause();
private:
  static constexpr uint32_t m_audio_buffer_size = 4096;

  QueueHandle_t m_cmd_queue;
  std::shared_ptr<fatfs::file_t> m_file;
  cmd_t m_current_cmd;
  size_t m_wav_header_size;
  uint8_t* m_audio_buffer;
  bool m_playing;
  std::string m_wav_name;
  int m_already_read_data_pos;

  bool open_wav_file(const std::string& a_filename);
};


#endif // READ_FILE_THREAD_H
