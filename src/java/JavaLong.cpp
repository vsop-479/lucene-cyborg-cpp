#include <algorithm>
#include "JavaLong.hpp"

using lucene::cyborg::java::JavaLong;

std::string JavaLong::to_string(int64_t i, int32_t radix) {
  std::string result;

  while (i > 0) {
    const auto digit = (i % radix);
    const char digit_char = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    result = digit_char;
    i /= radix;
  }

  if (!result.empty()) {
    std::reverse(result.begin(), result.end());
    return result;
  } else {
    return "0";
  }
}

int64_t JavaLong::parse_long(std::string_view s, int32_t radix) {
  int64_t res = 0;
  for (char c : s) {
    if (c >= 'a') {
      res = radix * res + (10 + (c - 'a'));
    } else {
      res = radix * res + (c - '0');
    }
  }
  return res;
}
