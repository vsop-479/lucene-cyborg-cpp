#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_TOTALHITS_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_TOTALHITS_HPP_

#include <cstdint>

namespace lucene::cyborg::search {

enum class TotalHitsRelation {
  // The total hit count is equal to TotalHits::value
  EQUAL_TO,
  // The total hit count is greater than or equal to TotalHits::value.
  GREATER_THAN_OR_EQUAL_TO
};  // TotalHitsRelation



struct TotalHits {
  TotalHitsRelation relation;
  int64_t value;
};  // TotalHits



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_TOTALHITS_HPP_
