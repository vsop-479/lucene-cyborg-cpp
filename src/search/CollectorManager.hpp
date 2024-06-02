#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTORMANAGER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTORMANAGER_HPP_

#include "ScoreMode.hpp"
#include "SearchContext.hpp"
#include "Collector.hpp"

namespace lucene::cyborg::search {

class CollectorManager {
 public:
  explicit CollectorManager(ScoreMode _score_mode)
      : score_mode(_score_mode) {
  }

  [[nodiscard]] ScoreMode get_score_mode() const noexcept {
    return score_mode;
  }

  virtual util::ManagedPtr<Collector> new_collector() = 0;

  virtual void merge(util::ManagedPtr<Collector> collectors, bool is_last) {
    // no ops
  }

 protected:
  ScoreMode score_mode;
};  // CollectorManager



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTORMANAGER_HPP_
