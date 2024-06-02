#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_BULKSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_BULKSCORER_HPP_

#include <cstdint>
#include <cassert>
#include <functional>
#include "../util/PointerTypes.hpp"
#include "LeafCollector.hpp"
#include "DocIdSetIterator.hpp"
#include "TwoPhaseIterator.hpp"

namespace lucene::cyborg::search {

class BulkScorer {
 public:
  virtual void score() {
    const int32_t next = score(0, DocIdSetIterator::NO_MORE_DOCS);
    assert(next == DocIdSetIterator::NO_MORE_DOCS);
  }

  virtual int32_t score(int32_t min_doc_id, int32_t max_doc_id) = 0;

  virtual int64_t get_cost() = 0;

  virtual util::ManagedPtr<LeafCollector> get_collector() = 0;

  virtual util::ManagedPtr<LeafCollector> reset_collector(util::ManagedPtr<LeafCollector> new_collector) = 0;
};  // BulkScorer



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_BULKSCORER_HPP_
