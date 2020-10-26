#ifndef APP_H
#define APP_H

#include "cfg.h"

class app_t
{
private:
  cfg_t *mp_cfg;
public:
    app_t();
    void tick();
};

#endif // APP_H
