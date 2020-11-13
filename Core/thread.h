#ifndef THREAD_H
#define THREAD_H

#include "FreeRTOS.h"
#include "task.h"
#include <cassert>


class thread_base_t
{
public:
  virtual void task() = 0;

  static void run(void* parm) {
    reinterpret_cast<thread_base_t*>(parm)->task();
    assert(false);
  }

  TaskHandle_t handle =  nullptr;
};


template<class T>
void create_thread(T& thread, char const* a_name, UBaseType_t priority, const configSTACK_DEPTH_TYPE stackDepth)
{
  if (xTaskCreate(static_cast<TaskFunction_t>(T::run), a_name, stackDepth, &thread, priority, &thread.handle) != pdPASS)
  {
    assert(false);
  }
}

#endif // THREAD_H
