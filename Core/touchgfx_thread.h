#ifndef TOUCHGFX_THREAD_H
#define TOUCHGFX_THREAD_H

#include "thread.h"

extern "C" void touchgfx_init();
extern "C" void touchgfx_taskEntry();

class touchgfx_thread_t : public thread_base_t
{
public:
  touchgfx_thread_t()
  {
    touchgfx_init();
  }

  void task() override
  {
    touchgfx_taskEntry();
  }
};



#endif // TOUCHGFX_THREAD_H
