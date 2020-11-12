#ifndef APP_H
#define APP_H

#include "cfg.h"
#include "touchgfx_thread.h"

class app_t
{
private:
  cfg_t *mp_cfg;

public:
    app_t();
  touchgfx_thread_t touchgfx_thread;
};

#endif // APP_H
