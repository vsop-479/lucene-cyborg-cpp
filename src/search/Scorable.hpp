#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCORABLE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCORABLE_HPP_

#include <cstdint>

namespace lucene::cyborg::search {

class Scorable {
 public:
  virtual float get_score() = 0;

  virtual float get_smoothing_score(int32_t doc_id) {
    return 0;
  }

  virtual int32_t get_doc_id() = 0;

  virtual void set_min_competitive_score(float min_score) {
    // No-op by default
  }
};  // Scorable



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCORABLE_HPP_
