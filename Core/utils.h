#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <deque>

namespace mem {

template <class T>
T* aligined_alloc(size_t a_align)
{
  void* p_mem = ::operator new(sizeof(T) + a_align);
  size_t aligned_address = (reinterpret_cast<size_t>(p_mem) & ~(a_align - 1)) + a_align;

  assert(aligned_address % a_align == 0);
  assert(aligned_address + sizeof(T) <= reinterpret_cast<size_t>(p_mem) + sizeof(T) + a_align);

  return reinterpret_cast<T*>(aligned_address);
}
}


template <class data_t, class calc_t>
class fast_average_t
{
public:
  typedef size_t size_type;
  fast_average_t();
  fast_average_t(size_type a_count);
  void add(data_t a_val);
  calc_t get() const;
  void resize(size_type a_count);
  size_type size() const;
  size_type max_size() const;
  bool is_full() const;
  void clear();
  void preset(size_type a_start_pos, size_type a_count);
private:
  size_type m_max_count;
  std::deque<data_t> m_samples;
  calc_t m_sum;
};

template <class data_t, class calc_t>
fast_average_t<data_t, calc_t>::fast_average_t():
  m_max_count(0),
  m_samples(),
  m_sum(0)
{

}

template <class data_t, class calc_t>
fast_average_t<data_t, calc_t>::fast_average_t(size_type a_count):
  m_max_count(a_count),
  m_samples(),
  m_sum(0)
{

}

template <class data_t, class calc_t>
void fast_average_t<data_t, calc_t>::add(data_t a_val)
{
  if (m_max_count > 0) {
    if (m_samples.size() == m_max_count) {
      m_sum -= m_samples.front();
      m_samples.pop_front();
    }
    m_samples.push_back(a_val);
    m_sum += a_val;
  }
}

template <class data_t, class calc_t>
calc_t fast_average_t<data_t, calc_t>::get() const
{
  if (m_samples.empty()) {
    return 0;
  }
  return m_sum/static_cast<calc_t>(m_samples.size());
}


template <class data_t, class calc_t>
void fast_average_t<data_t, calc_t>::resize(size_type a_count)
{
  m_max_count = a_count;
  while (m_samples.size() > m_max_count) {
    m_sum -= m_samples.front();
    m_samples.pop_front();
  }
}

template <class data_t, class calc_t>
typename fast_average_t<data_t, calc_t>::size_type
fast_average_t<data_t, calc_t>::size() const
{
  return m_samples.size();
}

template <class data_t, class calc_t>
typename fast_average_t<data_t, calc_t>::size_type
fast_average_t<data_t, calc_t>::max_size() const
{
  return m_max_count;
}

template <class data_t, class calc_t>
bool fast_average_t<data_t, calc_t>::is_full() const
{
  return m_samples.size() == m_max_count;
}

template <class data_t, class calc_t>
void fast_average_t<data_t, calc_t>::clear()
{
  m_samples.clear();
  m_sum = 0;
}

template <class data_t, class calc_t>
void fast_average_t<data_t, calc_t>::preset(
  size_type a_start_pos, size_type a_count)
{
  const size_type erase_front_count = min(m_samples.size(), a_start_pos);
  m_samples.erase(m_samples.begin(), m_samples.begin() + erase_front_count);
  const size_type erase_back_count = min(m_samples.size(),
    m_samples.size() - a_count);
  m_samples.erase(m_samples.end() - erase_back_count, m_samples.end());
  const size_type size = min(m_samples.size(), a_count);
  if (size > 0) {
    while (m_samples.size() < m_max_count) {
      for (size_type i = 0; i < size; i++) {
        m_samples.push_back(m_samples[i]);
        if (m_samples.size() == m_max_count) {
          break;
        }
      }
    }
  }
  m_sum = 0;
  for (size_type i = 0; i < m_samples.size(); i++) {
    m_sum += m_samples[i];
  }
}


#endif // UTILS_H
