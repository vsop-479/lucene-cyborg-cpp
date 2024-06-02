#ifndef LUCENE_CYBORG_CPP_SRC_PCO_UTIL_PCONULLTYPE_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_UTIL_PCONULLTYPE_HPP_

#include <type_traits>

namespace lucene::cyborg::pco {

struct PcoNullType final {
  template<typename T>
  static constexpr bool is_null_type() noexcept {
    return std::is_same_v<T, PcoNullType>;
  }
};  // PcoNullType



}  // namespace lucene::cyborg::pco

#endif //LUCENE_CYBORG_CPP_SRC_PCO_UTIL_PCONULLTYPE_HPP_
