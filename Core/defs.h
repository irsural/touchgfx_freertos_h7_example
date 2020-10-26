#include <iostream>

#define TEMPLATE_DEBUG  1



#if TEMPLATE_DEBUG
#define DBG_MSG(msg) { std::cout << msg << std::endl; }
#else
#define DBG_MSG(msg)
#endif // TEMPLATE_DEBUG