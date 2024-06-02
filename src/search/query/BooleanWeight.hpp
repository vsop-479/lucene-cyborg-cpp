#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANWEIGHT_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANWEIGHT_HPP_

#include "../Weight.hpp"
#include "../IndexSearcher.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "BooleanClause.hpp"
#include "BooleanQuery.hpp"

namespace lucene::cyborg::search::query {

class BooleanWeight final : public Weight {
 public:
  struct WeightedBooleanClause final : public BooleanClause {
    util::ManagedPtr<Weight> weight;
  };

  BooleanWeight(util::ManagedPtr<SearchContext> srch_context,
                util::UseOnlyPtr<BooleanQuery> boolean_query,
                util::UseOnlyPtr<IndexSearcher> searcher,
                ScoreMode score_mode,
                float boost = 0);

  util::ManagedPtr<BulkScorer> get_bulk_scorer(
      util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
      util::UseOnlyPtr<LeafCollector> leaf_collector,
      util::UseOnlyPtr<util::Bits> live_docs) final;

  util::ManagedPtr<Scorer> get_scorer(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) final;

  util::ManagedPtr<ScorerSupplier> get_scorer_supplier(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) final;

  ScoreMode score_mode;
  util::LVector<WeightedBooleanClause> weighted_clauses;
  util::ManagedPtr<Similarity> similarity;
  util::ManagedPtr<BooleanQuery> boolean_query;

 private:
  util::ManagedPtr<BulkScorer> get_boolean_scorer(
      util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
      util::ManagedPtr<LeafCollector> leaf_collector,
      util::ManagedPtr<util::Bits> live_docs);

  util::ManagedPtr<BulkScorer> required_bulk_scorer(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
                                                    util::ManagedPtr<LeafCollector> leaf_collector,
                                                    util::UseOnlyPtr<util::Bits> live_docs);

  util::ManagedPtr<BulkScorer> optional_bulk_scorer(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
                                                    util::ManagedPtr<LeafCollector> leaf_collector,
                                                    util::ManagedPtr<util::Bits> live_docs);
};  // BooleanWeight



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANWEIGHT_HPP_
