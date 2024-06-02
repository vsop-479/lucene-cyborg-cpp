#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERUTIL_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERUTIL_HPP_

#include <cstdint>

namespace lucene::cyborg::search {

struct ScorerUtil {
  static int64_t cost_with_min_should_match(int64_t costs[], int32_t num_scorers, int32_t min_should_match) noexcept;
};  // ScorerUtil



}  // lucene::cyborg::search


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERUTIL_HPP_
