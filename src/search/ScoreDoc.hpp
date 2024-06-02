#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREDOC_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREDOC_HPP_

#include <cstdint>

namespace lucene::cyborg::search {

struct ScoreDoc {
  float score;
  int32_t doc;
  int32_t shard_index;
};  // ScoreDoc



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREDOC_HPP_
