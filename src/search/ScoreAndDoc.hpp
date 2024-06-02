#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREANDDOC_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREANDDOC_HPP_

#include "Scorable.hpp"

namespace lucene::cyborg::search {

struct ScoreAndDoc final : public Scorable {
 public:
  float get_score() noexcept final {
    return score;
  }

  int32_t get_doc_id() noexcept final {
    return doc;
  }

  float score;
  int32_t doc;
};  // ScoreAndDoc



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREANDDOC_HPP_
