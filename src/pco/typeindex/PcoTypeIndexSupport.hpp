#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_PCO_PCOTYPEINDEXSUPPORT_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_PCO_PCOTYPEINDEXSUPPORT_HPP_

#include "PcoObjectTypeIndex.hpp"
#include "PcoTemplatedObjectTypeIndex.hpp"
#include "PcoNativeTypeIndex.hpp"
#include <cstdint>

namespace lucene::cyborg::pco {

class PcoTypeIndexSupport {
 public:
  explicit PcoTypeIndexSupport(int32_t _pco_type_index)
      : pco_type_index(_pco_type_index) {
  }

  [[nodiscard]] virtual int32_t get_pco_type_index() const noexcept {
    return pco_type_index;
  }

  bool operator==(const PcoTypeIndexSupport &rhs) const noexcept {
    return pco_type_index == rhs.pco_type_index;
  }

  bool operator!=(const PcoTypeIndexSupport &rhs) const noexcept {
    return !(rhs == *this);
  }

 protected:
  int32_t pco_type_index;
};  // PcoTypeIndexSupport

using PcoTypeIndexSupportPtr = const PcoTypeIndexSupport *;

}  // namespace lucene::cyborg::pco

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_PCO_PCOTYPEINDEXSUPPORT_HPP_
