#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTOR_HPP_

#include "SearchContext.hpp"
#include "LeafCollector.hpp"
#include "ScoreMode.hpp"

namespace lucene::cyborg::index {
struct LeafReaderContext;
}

namespace lucene::cyborg::search {

class Weight;

struct Collector {
  explicit Collector(util::ManagedPtr<SearchContext> _srch_context)
      : srch_context(_srch_context) {
  }

  virtual util::ManagedPtr<LeafCollector> get_leaf_collector(util::ManagedPtr<index::LeafReaderContext> leaf_reader_context) = 0;

  void set_weight(util::ManagedPtr<Weight> _weight) noexcept {
    weight = _weight;
  }

 protected:
  ScoreMode score_mode;
  util::ManagedPtr<SearchContext> srch_context;
  util::ManagedPtr<Weight> weight;
};  // Collector



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_COLLECTOR_HPP_
