#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMQUERY_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMQUERY_HPP_

#include <optional>
#include "Query.hpp"
#include "../Weight.hpp"
#include "../ScorerSupplier.hpp"
#include "TermStates.hpp"
#include "../Similarity.hpp"
#include "../LeafSimScorer.hpp"
#include "../ImpactsDisi.hpp"
#include "../ImpactsDISIPco.hpp"

namespace lucene::cyborg::search::query {

class TermWeight final : public Weight {
 public:
  // For the aggregation
  TermWeight(util::ManagedPtr<SearchContext> srch_ctx, ScoreMode score_mode, util::ManagedPtr<TermStates> term_states);

  // For searching
  TermWeight(util::UseOnlyPtr<IndexSearcher> searcher,
             util::ManagedPtr<SearchContext> srch_ctx,
             ScoreMode score_mode,
             float boost,
             util::ManagedPtr<TermStates> term_states,
             const std::string_view &field_name,
             const std::string_view &term);

  util::ManagedPtr<ScorerSupplier> get_scorer_supplier(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) final;

  util::ManagedPtr<Scorer> get_scorer(util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context) final;

  util::ManagedPtr<BulkScorer> get_bulk_scorer(
      util::UseOnlyPtr<index::LeafReaderContext> leaf_reader_context,
      util::UseOnlyPtr<LeafCollector> leaf_collector,
      util::UseOnlyPtr<util::Bits> live_docs) final;

  // Common
  ScoreMode score_mode;
  util::ManagedPtr<TermStates> term_states;
  // For searching
  util::ManagedPtr<Similarity> similarity;
  util::ManagedPtr<SimScorer> sim_scorer;
  std::string_view field_name;
  std::string_view term;
};  // TermWeight



class TermScorer final : public Scorer {
 public:
  // For the aggregation
  TermScorer(util::ManagedPtr<TermWeight> term_weight,
             util::ManagedPtr<index::PostingsEnum> postings_enum);

  // For searching
  TermScorer(util::ManagedPtr<TermWeight> term_weight,
             util::ManagedPtr<index::ImpactsEnum> impacts_enum,
             const LeafSimScorer &doc_scorer,
             bool top_level_scoring_clause);

  float get_score() final;

  float get_smoothing_score(int32_t doc_id) final;

  int32_t get_doc_id() final;

  void set_min_competitive_score(float min_score) final;

  int32_t advance_shallow(int32_t target) final;

  float get_max_score(int32_t upto) final;

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

 private:
  // Common
  util::ManagedPtr<index::PostingsEnum> postings_enum;
  util::ManagedPtr<DocIdSetIterator> iterator;

  // For searching
  std::optional<MaxScoreCache> max_score_cache;
  std::optional<LeafSimScorer> doc_scorer;
  util::ManagedPtr<ImpactsDISIPcoBase> impacts_disi;
};  // TermScorer



class TermScorerSupplier final : public ScorerSupplier {
 public:
  TermScorerSupplier(util::ManagedPtr<TermWeight> term_weight,
                     util::ManagedPtr<index::TermsEnum> terms_enum,
                     util::ManagedPtr<index::SegmentReader> segment_reader);

  util::ManagedPtr<TermScorer> get(int64_t lead_cost) final;

  int64_t get_cost() final {
    return doc_freq;
  }

  void set_top_level_scoring_clause() final {
    top_level_scoring_clause = true;
  }

 private:
  int32_t doc_freq;
  ScoreMode score_mode;
  bool top_level_scoring_clause;
  util::ManagedPtr<index::TermsEnum> terms_enum;
  util::ManagedPtr<TermWeight> term_weight;
  util::ManagedPtr<index::SegmentReader> segment_reader;
};  // TermScorerSupplier



class TermQuery final : public Query {
 public:
  static const QueryIdentifier QUERY_IDENTIFIER;

  TermQuery(std::string_view field, std::string_view term);

  util::ManagedPtr<Weight> create_weight(util::UseOnlyPtr<SearchContext> srch_context,
                                         util::ManagedPtr<IndexSearcher> index_searcher,
                                         ScoreMode score_mode,
                                         float boost) final;

  uint64_t hash() const noexcept final;

  bool equals(util::ReadOnlyPtr<Query> other) const noexcept final;

 private:
  std::string_view field;
  std::string_view term;
  util::ManagedPtr<TermStates> ephemeral_term_states;
};  // TermQuery



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMQUERY_HPP_
