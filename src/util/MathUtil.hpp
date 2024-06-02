#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_MATHUTIL_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_MATHUTIL_HPP_

#include <cstdint>

namespace lucene::cyborg::util {

struct MathUtil {
  static double sum_upper_bound(double sum, int32_t num_values);

  static double sum_relative_error_bound(int32_t num_values);

 private:
  MathUtil() = default;
};  // MathUtil



}  // namespace lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_MATHUTIL_HPP_
