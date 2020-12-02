#include "fft_thread.h"
#include "cfg.h"


fft_thread_t::fft_thread_t(SemaphoreHandle_t a_samples_ready_smph, SemaphoreHandle_t a_samples_processed_smph,
    SemaphoreHandle_t a_fft_done_smph) :
  m_samples_ready_smph(a_samples_ready_smph),
  m_samples_processed_smph(a_samples_processed_smph),
  m_fft_done_smph(a_fft_done_smph),
  m_samples(samples_buffer_size),
  m_harmonics_avg(),
  m_harmonics(),
  m_normalize_value(2e4)
{
  for (auto& frequency: m_harmonics_avg) {
    frequency.resize(frequency_average_size);
  }
}

fft_thread_t::~fft_thread_t()
{
  vTaskDelete(m_handle);
}

void fft_thread_t::task()
{
  xSemaphoreGive(m_samples_processed_smph);
  while(1) {
    if (xSemaphoreTake(m_samples_ready_smph, portMAX_DELAY) == pdTRUE) {

      for (size_t samples_pack = 0; samples_pack < samples_buffer_size / fft_size_samples; samples_pack += 1) {

        size_t pack_first_sample = samples_pack * fft_size_samples;
        size_t pack_last_sample = (samples_pack + 1) * fft_size_samples;

        do_fft(pack_first_sample, pack_last_sample);

        for (size_t i = first_visible_harmonic; i < m_harmonics_avg.size(); ++i) {
          m_harmonics_avg[i].add(m_samples[i + pack_first_sample]);
        }
      }
      xSemaphoreGive(m_samples_processed_smph);

      if (m_harmonics_avg[first_visible_harmonic].is_full()) {
        for (size_t harmonic_num = 0; harmonic_num < harmonics_count; ++harmonic_num) {
          float harmonic_value = abs(m_harmonics_avg[harmonic_num].get());

//        if (harmonic_value * 1.1 > m_normalize_value) {
//          m_normalize_value = harmonic_value * 1.1;
//        }
          m_harmonics[harmonic_num] = harmonic_value / m_normalize_value * 100;
        }

        xSemaphoreGive(m_fft_done_smph);
      }
    }
  }
}

std::vector<std::complex<float>>* fft_thread_t::get_samlpes_buffer()
{
  return &m_samples;
}

void fft_thread_t::do_fft(size_t from, size_t to)
{
  int degree = 0;
  int samples_size = to - from;
  // Ассерт на степень двойки
  assert(samples_size && ((samples_size & (samples_size - 1)) == 0));

  for (int i = samples_size; i >>= 1;) degree++;
  int size = 1 << degree;

  for (int first_idx = 0; first_idx < samples_size; first_idx++)
  {
    int second_idx = 0;
    for (int create_idx = 0; create_idx < degree; create_idx++) {
      second_idx |= ((first_idx >> (degree - create_idx - 1))&1) << create_idx;
    }
    if (second_idx > first_idx) {
      std::swap(m_samples[first_idx + from], m_samples[second_idx + from]);
    }
  }

  std::complex<float> divider = -1.;
  int group_size = size/2;
  int butterfly_size = 1;
  for (int stage_idx = 0; stage_idx < degree; stage_idx++)
  {
    for (int group_idx = 0; group_idx < group_size; group_idx++)
    {
      std::complex<float> factor = 1.;
      for (int butterfly_idx = 0; butterfly_idx < butterfly_size; butterfly_idx++)
      {
        int wing_first = 2*group_idx*butterfly_size + butterfly_idx + from;
        int wing_second = wing_first + butterfly_size;
        m_samples[wing_second] *= factor;
        std::complex<float> signal_add = m_samples[wing_first] + m_samples[wing_second];
        std::complex<float> signal_sub = m_samples[wing_first] - m_samples[wing_second];
        m_samples[wing_first] = signal_add;
        m_samples[wing_second] = signal_sub;
        factor /= divider;
      }
    }
    divider = sqrt(divider);
    group_size /= 2;
    butterfly_size *= 2;
  }
}
