#include "codecvt_oem866.h"
#include <iterator>


std::vector<touchgfx::Unicode::UnicodeChar> oem866_to_unicode(const char* a_str)
{
  codecvt_cp866 conv;
  mbstate_t mb;
  // Нулевой символ тоже конвертируем
  size_t str_len = strlen(a_str) + 1;
  const char* from_next = 0;
  std::vector<touchgfx::Unicode::UnicodeChar> to(str_len * 2, 0);
  uint16_t* to_next = 0;

  conv.in(mb, a_str, a_str + str_len, from_next, to.data(),
    to.data() + str_len, to_next);

  return to;
}