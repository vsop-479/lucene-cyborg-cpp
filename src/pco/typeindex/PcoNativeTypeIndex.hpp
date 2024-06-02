#ifndef LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_

#include <cstdint>
#include "../../index/IndexOptions.hpp"

namespace lucene::cyborg::pco {

template<typename>
struct PcoNativeTypeIndex;

template<>
struct PcoNativeTypeIndex<lucene::cyborg::index::IndexOptions> {
  static int32_t get_index(const lucene::cyborg::index::IndexOptions value) noexcept {
    if (value == lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS) {
      return 0;
    } else if (value == lucene::cyborg::index::IndexOptions::DOCS) {
      return 1;
    } else if (value == lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS) {
      return 2;
    } else if (value == lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
      return 3;
    } else if (value == lucene::cyborg::index::IndexOptions::NONE) {
      return 4;
    }
    return -1;
  }
};  // lucene::cyborg::index::IndexOptions



}  // namespace lucene::cyborg::pco

#endif //LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCONATIVETYPEINDEX_HPP_
