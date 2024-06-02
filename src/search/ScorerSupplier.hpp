#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERSUPPLIER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERSUPPLIER_HPP_

#include <cstdint>
#include "Scorer.hpp"
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::search {

class ScorerSupplier {
 public:
  virtual util::ManagedPtr<Scorer> get(int64_t lead_cost) = 0;

  virtual int64_t get_cost() = 0;

  virtual void set_top_level_scoring_clause() {
    // no-op
  }
};  // ScorerSupplier



}  // namespace lc::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCORERSUPPLIER_HPP_
