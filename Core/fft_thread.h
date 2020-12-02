#ifndef FFT_THREAD_H
#define FFT_THREAD_H

#include "thread.h"
#include "file_operations.h"
#include "utils.h"

#include <vector>
#include <array>
#include <complex>

#include <touchgfx/Unicode.hpp>


class fft_thread_t : public thread_base_t
{
public:
  using sample_t = uint16_t;
  static constexpr size_t fft_size_samples = 128;
  static constexpr size_t harmonics_count = 32;

  fft_thread_t(SemaphoreHandle_t a_samples_ready_smph, SemaphoreHandle_t a_samples_processed_smph,
    SemaphoreHandle_t a_fft_done_smph);
  ~fft_thread_t();
  virtual void task() override;


  std::vector<std::complex<float>>* get_samlpes_buffer();
  const std::array<float, harmonics_count>& get_harmonics() { return m_harmonics; };
private:
  static constexpr size_t samples_buffer_size = 512;
  static constexpr size_t frequency_average_size = 48;
  static_assert(samples_buffer_size % fft_size_samples == 0);
  static constexpr size_t first_visible_harmonic = 2;

  SemaphoreHandle_t m_samples_ready_smph;
  SemaphoreHandle_t m_samples_processed_smph;
  SemaphoreHandle_t m_fft_done_smph;

  std::vector<std::complex<float>> m_samples;
  // Симметричная сторона и и нулевая гармоника не нужны
  std::array<fast_average_t<std::complex<float>, std::complex<float>>, harmonics_count> m_harmonics_avg;
  std::array<float, harmonics_count> m_harmonics;
  float m_normalize_value;

  void do_fft(size_t from, size_t to);
};


#endif // FFT_THREAD_H
