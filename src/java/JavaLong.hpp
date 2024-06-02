#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JAVALONG_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JAVALONG_HPP_

#include <cstdint>
#include <string>

namespace lucene::cyborg::java {

class JavaLong {
 public:
  static constexpr int32_t MAX_RADIX = 36;

  static std::string to_string(int64_t i, int32_t radix);

  static int64_t parse_long(std::string_view s, int32_t radix);
};  // JavaLong



}  // lucene::cyborg::java

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JAVALONG_HPP_
