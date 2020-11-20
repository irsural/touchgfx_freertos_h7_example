#ifndef UTILS_H
#define UTILS_H

#include <cassert>

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


#endif // UTILS_H
