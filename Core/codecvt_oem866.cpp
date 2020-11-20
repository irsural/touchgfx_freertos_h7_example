#include "codecvt_oem866.h"
#include <iterator>


static codecvt_cp866 conv;

std::vector<touchgfx::Unicode::UnicodeChar> oem866_to_unicode(const char* a_str)
{
  mbstate_t mb;
  // Нулевой символ тоже конвертируем
  size_t str_len = strlen(a_str) + 1;
  const char* from_next = 0;

  std::vector<touchgfx::Unicode::UnicodeChar> to(str_len, 0);
  uint16_t* to_next = 0;

  conv.in(mb, a_str, a_str + str_len, from_next, to.data(), to.data() + str_len, to_next);

  return to;
}

std::string unicode_to_oem866(const std::vector<touchgfx::Unicode::UnicodeChar>& a_unicode_str)
{
  mbstate_t mb;
  // Нулевой символ тоже конвертируем
  size_t str_len = a_unicode_str.size();
  const uint16_t* from_next = 0;

  char* to = new char[str_len]();
  char* to_next = 0;

  conv.out(mb, a_unicode_str.data(), a_unicode_str.data() + str_len, from_next, to, to + str_len, to_next);

  return to;
}
