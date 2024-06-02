#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_WEIGHT_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_WEIGHT_HPP_

#include <optional>
#include "Scorer.hpp"
#include "ScorerSupplier.hpp"
#include "BulkScorer.hpp"
#include "SearchContext.hpp"
#include "LeafCollector.hpp"
#include "../util/PointerTypes.hpp"
#include "../util/Bits.hpp"
#include "../index/LeafReaderContext.hpp"

namespace lucene::cyborg::search {

struct DefaultScorerSupplier final : public ScorerSupplier {
  explicit DefaultScorerSupplier(util::ManagedPtr<Scorer> _scorer)
      : ScorerSupplier(),
        scorer(_scorer) {
  }

  Scorer *get(int64_t lead_cost) final {
    return scorer;
  }

  int64_t get_cost() final {
    if (cost_cached) {
      return *cost_cached;
    } else {
      return *(cost_cached = scorer->get_iterator()->get_cost());
    }
  }

  util::ManagedPtr<Scorer> scorer;
  std::optional<int64_t> cost_cached;
};  // DefaultScorerSupplier

class Weight {
 public:
  explicit Weight(util::ManagedPtr<SearchContext> _srch_context)
      : srch_context(_srch_context) {
  }

  virtual util::ManagedPtr<Scorer> get_scorer(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) = 0;

  virtual util::ManagedPtr<ScorerSupplier> get_scorer_supplier(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context);

  virtual util::ManagedPtr<BulkScorer> get_bulk_scorer(
      util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
      util::UseOnlyPtr<LeafCollector> leaf_collector,
      util::UseOnlyPtr<util::Bits> live_docs);

  virtual int32_t count(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) {
    return -1;
  }

  util::ManagedPtr<SearchContext> srch_context;
};  // Weight



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_WEIGHT_HPP_
