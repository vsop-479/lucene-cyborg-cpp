#ifndef LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_

#include <cstdint>

namespace lucene::cyborg::pco {

struct PcoObjectTypeIndex {
  static constexpr int32_t LUCENE_CYBORG_UTIL_BITS = 5;
  static constexpr int32_t LUCENE_CYBORG_SEARCH_DEFAULT_COUNT_DOC_ID_STREAM = 6;
  static constexpr int32_t LUCENE_CYBORG_SEARCH_TWO_PHASE_ITERATOR = 7;
  static constexpr int32_t LUCENE_CYBORG_PCO_PCO_NULL_TYPE = 8;
  static constexpr int32_t LUCENE_CYBORG_SEARCH_MATCH_COUNT_LEAF_COLLECTOR = 9;
  static constexpr int32_t LUCENE_CYBORG_SEARCH_DOC_ID_CONSUMER = 10;
  static constexpr int32_t LUCENE_CYBORG_SEARCH_DOC_ID_SET_ITERATOR = 11;
};  // PcoObjectTypeIndex



}  // namespace lucene::cyborg::pco

#endif //LUCENE_CYBORG_CPP_SRC_PCO_TYPEINDEX_PCOOBJECTTYPEINDEX_HPP_
