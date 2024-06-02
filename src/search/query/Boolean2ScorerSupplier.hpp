#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEAN2SCORERSUPPLIER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEAN2SCORERSUPPLIER_HPP_

#include "../ScorerSupplier.hpp"
#include "BooleanClause.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"

namespace lucene::cyborg::search::query {

class Boolean2ScorerSupplier final : public ScorerSupplier {
 public:
  Boolean2ScorerSupplier(util::ManagedPtr<Weight> weight,
                         OccurEnumMap<util::LVector<util::ManagedPtr<ScorerSupplier>>> &&scorer_suppliers,
                         ScoreMode score_mode,
                         int32_t min_should_match);

  util::ManagedPtr<Scorer> get(int64_t lead_cost) final;

  int64_t get_cost() final;

  void set_top_level_scoring_clause() final;

 private:
  util::ManagedPtr<Scorer> do_get_scorer(int64_t lead_cost);

  int64_t compute_cost();

  util::ManagedPtr<Scorer> exclude(util::ManagedPtr<Scorer> main,
                                   util::LVector<util::ManagedPtr<ScorerSupplier>> &prohibited,
                                   int64_t lead_cost);

  util::ManagedPtr<Scorer> require(util::LVector<util::ManagedPtr<ScorerSupplier>> &required_no_scoring,
                                   util::LVector<util::ManagedPtr<ScorerSupplier>> &required_scoring,
                                   int64_t lead_cost,
                                   bool top_level_scoring_clause);

  util::ManagedPtr<Scorer> optional(
      util::LVector<util::ManagedPtr<ScorerSupplier>> &optional,
      int32_t min_should_match,
      ScoreMode score_mode,
      int64_t lead_cost,
      bool top_level_scoring_clause);

  util::ManagedPtr<Weight> weight;
  OccurEnumMap<util::LVector<util::ManagedPtr<ScorerSupplier>>> scorer_suppliers;
  ScoreMode score_mode;
  int32_t min_should_match;
  int64_t cost;
  bool top_level_scoring_clause;
};  // Boolean2ScorerSupplier



}  // lucene::cyborg::search::query


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEAN2SCORERSUPPLIER_HPP_
