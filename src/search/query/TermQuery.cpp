#include <functional>
#include <limits>
#include "TermQuery.hpp"
#include "../IndexSearcher.hpp"
#include "../../pco/factory/PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco.hpp"
#include "../../pco/factory/PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco.hpp"

using lucene::cyborg::index::ImpactsEnum;
using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::SegmentReader;
using lucene::cyborg::index::TermsEnum;
using lucene::cyborg::pco::PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco;
using lucene::cyborg::pco::PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco;
using lucene::cyborg::search::BulkScorer;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::LeafSimScorer;
using lucene::cyborg::search::QueryIdentifier;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::ScorerSupplier;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::TermQuery;
using lucene::cyborg::search::query::TermScorer;
using lucene::cyborg::search::query::TermScorerSupplier;
using lucene::cyborg::search::query::TermWeight;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// TermScorer
//

// For the aggregation
TermScorer::TermScorer(ManagedPtr<TermWeight> _term_weight,
                       ManagedPtr<PostingsEnum> _postings_enum)
    : Scorer(_term_weight),
      postings_enum(_postings_enum),
      iterator(_postings_enum) {
}

// For searching
TermScorer::TermScorer(ManagedPtr<TermWeight> _term_weight,
                       ManagedPtr<ImpactsEnum> _impacts_enum,
                       const LeafSimScorer &_doc_scorer,
                       bool _top_level_scoring_clause)
    : Scorer(_term_weight),
      postings_enum(_impacts_enum),
      iterator(),
      max_score_cache(MaxScoreCache{_term_weight->srch_context->linear_allocator,
                                    _impacts_enum,
                                    _doc_scorer.scorer}),
      doc_scorer(_doc_scorer),
      impacts_disi() {
  if (_top_level_scoring_clause) {
    impacts_disi = PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco::create(
        _impacts_enum,
        &(*max_score_cache),
        _term_weight->srch_context->linear_allocator);
    iterator = impacts_disi;
  } else {
    iterator = _impacts_enum;
  }
}

int32_t TermScorer::advance_shallow(int32_t target) {
  return (*max_score_cache).advance_shallow(target);
}

float TermScorer::get_max_score(int32_t upto) {
  return (*max_score_cache).get_max_score(upto);
}

ManagedPtr<DocIdSetIterator> TermScorer::get_iterator() {
  return iterator;
}

float TermScorer::get_score() {
  assert(get_doc_id() != DocIdSetIterator::NO_MORE_DOCS);
  return (*doc_scorer).get_score(postings_enum->get_doc_id(), (float) postings_enum->get_freq());
}

float TermScorer::get_smoothing_score(int32_t doc_id) {
  return (*doc_scorer).get_score(doc_id, 0);
}

int32_t TermScorer::get_doc_id() {
  return postings_enum->get_doc_id();
}

void TermScorer::set_min_competitive_score(float min_score) {
  if (impacts_disi) {
    impacts_disi->set_min_competitive_score(min_score);
  }
}

//
// TermScorerSupplier
//

TermScorerSupplier::TermScorerSupplier(ManagedPtr<TermWeight> _term_weight,
                                       ManagedPtr<TermsEnum> _terms_enum,
                                       ManagedPtr<SegmentReader> _segment_reader)
    : ScorerSupplier(),
      doc_freq(_terms_enum->doc_freq()),
      score_mode(_term_weight->score_mode),
      top_level_scoring_clause(),
      terms_enum(_terms_enum),
      term_weight(_term_weight),
      segment_reader(_segment_reader) {
}

ManagedPtr<TermScorer> TermScorerSupplier::get(int64_t lead_cost) {
  if (score_mode == ScoreMode::COMPETE_NO_SCORES) {
    // It's an aggregation
    return term_weight->srch_context->linear_allocator->allocate_object<TermScorer>(
        term_weight, terms_enum->postings(PostingsEnum::Flags::NONE));
  } else {
    // For searching
    return term_weight->srch_context->linear_allocator->allocate_object<TermScorer>(
        term_weight,
        terms_enum->impacts(PostingsEnum::Flags::FREQS),
        LeafSimScorer{term_weight->srch_context->linear_allocator,
                      term_weight->sim_scorer,
                      segment_reader,
                      term_weight->field_name},
        top_level_scoring_clause);
  }  // End if
}

//
// TermQuery
//

const QueryIdentifier TermQuery::QUERY_IDENTIFIER{"TermQuery"};

TermQuery::TermQuery(std::string_view _field, std::string_view _term)
    : Query(&QUERY_IDENTIFIER),
      field(_field),
      term(_term),
      ephemeral_term_states() {
}

ManagedPtr<Weight> TermQuery::create_weight(UseOnlyPtr<SearchContext> srch_context,
                                            ManagedPtr<IndexSearcher> index_searcher,
                                            ScoreMode score_mode,
                                            float boost) {
  if (ephemeral_term_states == nullptr) {
    ephemeral_term_states = TermStates::build(srch_context,
                                              index_searcher->get_leaf_reader_contexts(),
                                              field,
                                              term,
                                              score_mode_need_scores(score_mode));
  }

  return srch_context->linear_allocator->allocate_object<TermWeight>(
      index_searcher,
      srch_context,
      score_mode,
      boost,
      ephemeral_term_states,
      field,
      term);
}

uint64_t TermQuery::hash() const noexcept {
  std::hash<std::string_view> hasher{};
  return class_name_hash ^ (31ULL + 31ULL * hasher(field) + (31ULL * 31ULL) * hasher(term));
}

bool TermQuery::equals(ReadOnlyPtr<Query> other) const noexcept {
  if (Query::equals(other)) {
    auto other_term_query = (ReadOnlyPtr<TermQuery>) other;
    return field == other_term_query->field && term == other_term_query->term;
  }
  return false;
}

//
// TermWeight
//

// For the aggregation
TermWeight::TermWeight(ManagedPtr<SearchContext> _srch_ctx, ScoreMode _score_mode, ManagedPtr<TermStates> _term_states)
    : Weight(_srch_ctx),
      score_mode(_score_mode),
      term_states(_term_states) {
}

// For searching
TermWeight::TermWeight(UseOnlyPtr<IndexSearcher> _searcher,
                       ManagedPtr<SearchContext> _srch_ctx,
                       ScoreMode _score_mode,
                       float _boost,
                       ManagedPtr<TermStates> _term_states,
                       const std::string_view &_field_name,
                       const std::string_view &_term)
    : Weight(_srch_ctx),
      score_mode(_score_mode),
      term_states(_term_states),
      similarity(_searcher->get_similarity()),
      sim_scorer(),
      field_name(_field_name),
      term(_term) {
  if (_term_states == nullptr) [[unlikely]] {
    // TODO
    throw 13;
  }

  if (score_mode_need_scores(_score_mode) && _term_states->doc_freq > 0) {
    auto term_stats = _searcher->get_term_statistics(_term, _term_states->doc_freq, _term_states->total_term_freq);
    auto collection_stats = _searcher->get_collection_statistics(_srch_ctx, _field_name);
    sim_scorer = similarity->get_scorer(_srch_ctx, _boost, collection_stats, &term_stats, 1);
  }  // End if
}

ManagedPtr<ScorerSupplier> TermWeight::get_scorer_supplier(UseOnlyPtr<LeafReaderContext> leaf_reader_context) {
  assert(leaf_reader_context->ord >= 0);
  assert(leaf_reader_context->ord < term_states->terms_enums.len);

  auto terms_enum = term_states->terms_enums.vec[leaf_reader_context->ord];
  if (terms_enum) {
    return srch_context->linear_allocator->allocate_object<TermScorerSupplier>(
        this, terms_enum, leaf_reader_context->segment_reader);
  } else {
    // term was not found
    return nullptr;
  }  // End if
}

ManagedPtr<Scorer> TermWeight::get_scorer(UseOnlyPtr<LeafReaderContext> leaf_reader_context) {
  if (auto scorer_supplier = get_scorer_supplier(leaf_reader_context)) {
    return scorer_supplier->get(std::numeric_limits<int64_t>::max());
  } else {
    return nullptr;
  }  // End if
}

ManagedPtr<BulkScorer> TermWeight::get_bulk_scorer(
    UseOnlyPtr<LeafReaderContext> leaf_reader_context,
    UseOnlyPtr<LeafCollector> leaf_collector,
    UseOnlyPtr<Bits> live_docs) {
  if (auto scorer_supplier = get_scorer_supplier(leaf_reader_context)) {
    scorer_supplier->set_top_level_scoring_clause();
    auto scorer = scorer_supplier->get(std::numeric_limits<int64_t>::max());

    return PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco::create_for_search(
        scorer->get_iterator(),
        scorer->two_phase_iterator(),
        live_docs,
        scorer,
        leaf_collector,
        srch_context->linear_allocator
    );
  } else {
    // No docs match
    return nullptr;
  }
}
