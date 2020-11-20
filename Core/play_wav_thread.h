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
  enum class cmd_type_t {
    none,
    play,
    pause,
    resume,
    set_position,
    set_volume
  };

  struct cmd_t
  {
    cmd_type_t type;
    int value;
  };

  enum class play_state_t {
    playing,
    paused,
    stopped
  };

  static constexpr double eof_value = -1;

  using char_type = touchgfx::Unicode::UnicodeChar;

  play_wav_thread_t(QueueHandle_t a_cmd_queue, QueueHandle_t a_percents_played_queue);
  ~play_wav_thread_t();
  virtual void task() override;

  void set_wav_file(std::string a_filename);
  const std::string& get_wav_file();

  bool play();
  bool resume();
  void pause();
  void stop();

  play_state_t get_play_state();
private:
  static constexpr uint32_t m_audio_buffer_size = 4096;
  // Каждые send_played_length_frequency байт будет отправляться сообщение
  static constexpr uint32_t send_played_length_frequency = 8192;

  QueueHandle_t m_cmd_queue;
  std::shared_ptr<fatfs::file_t> m_file;
  cmd_type_t m_current_cmd_type;
  size_t m_wav_header_size;
  uint8_t* m_audio_buffer;
  play_state_t m_play_state;
  std::string m_wav_name;
  int m_already_read_data_pos;
  size_t m_track_size;
  //Сбрасывается, когда превышает send_played_length_frequency, в отличии от m_bytes_played_percents
  size_t m_bytes_read;
  double m_bytes_played_percents;
  QueueHandle_t m_percents_played_queue;

  bool open_wav_file(const std::string& a_filename);
};


#endif // READ_FILE_THREAD_H
