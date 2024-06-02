#include <limits>
#include "BooleanWeight.hpp"
#include "Boolean2ScorerSupplier.hpp"
#include "DisjunctionSumScorer.hpp"
#include "BooleanReqExclBulkScorer.hpp"
#include "BooleanScorer.hpp"
#include "MaxScoreBulkScorer.hpp"

using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::search::BulkScorer;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::ScorerSupplier;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::Boolean2ScorerSupplier;
using lucene::cyborg::search::query::BooleanWeight;
using lucene::cyborg::search::query::MaxScoreBulkScorer;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;

/*
 * # BulkScorer - positive
 *
 * ## Case 1. |Required| == 0
 *  - msm > 1 AND get_cost < (|MaxDoc| / 3)
 *    - then set positive as optionalBulkScorer
 *      - MaxScoreBulkScorer
 *      - BooleanScorer
 *
 * ## Case 2. |Should| == 0 AND |Required| == 1 AND msm == 0
 *  - then set the positive as requiredBulkScorer
 *
 * # BulkScorer - prohibited
 *  - empty
 *    - then return the positive
 *  - size == 1
 *    - return ReqExclBulkScorer(positive, prohibited[0])
 *  - otherwise
 *    - if two phase iterator was involved in the prohibited iterator
 *      - forgive creating a bulk scorer
 *    - return ReqExclBulkScorer(positive, DisjunctionSumScorer(prohibited)) otherwise
 *
 * # ScorerSupplier
 * ## Case 1. excl(req(filter, must), must_not)
 *  - |Should| == 0
 * ## Case 2. excl(opt, must_not)
 *  - |Filter| == 0 && |MUST| == 0
 * ## Case 3. ConjunctionScorer(excl(req(filter, must), must_not),
 *                              opt)
 *  - msm > 0
 *  - |Should| > 0 && (|Filter| > 0 OR |MUST| > 0)
 * ## Case 4. ReqOptSumScorer(excl(req(filter, must), must_not),
 *                            opt)
 *  - msm <= 0
 *  - |Should| > 0 && (|Filter| > 0 OR |MUST| > 0)
 *
 * # SS req, excl, opt
 *  - req : ConjunctionScorer(weight, required_scorers, scoring_scorers)
 */

//
// BooleanWeight
//

BooleanWeight::BooleanWeight(ManagedPtr<SearchContext> _srch_context,
                             UseOnlyPtr<BooleanQuery> _boolean_query,
                             UseOnlyPtr<IndexSearcher> _searcher,
                             ScoreMode _score_mode,
                             float _boost)
    : Weight(_srch_context),
      score_mode(_score_mode),
      weighted_clauses(LinearAllocatorAdapterForSTL<WeightedBooleanClause>(_srch_context->linear_allocator)),
      similarity(_searcher->get_similarity()),
      boolean_query(_boolean_query) {
  weighted_clauses.reserve(_boolean_query->clauses.len);
  for (int32_t i = 0; i < _boolean_query->clauses.len; ++i) {
    auto &clause = _boolean_query->clauses.vec[i];
    auto weight = _searcher->create_weight(_srch_context,
                                           clause.query,
                                           clause.is_scoring() ? score_mode : ScoreMode::COMPETE_NO_SCORES,
                                           _boost);
    weighted_clauses.emplace_back(
        WeightedBooleanClause{clause.query, clause.occur, weight});
  }  // End for
}

ManagedPtr<BulkScorer> BooleanWeight::get_bulk_scorer(UseOnlyPtr<LeafReaderContext> leaf_reader_context,
                                                      UseOnlyPtr<LeafCollector> leaf_collector,
                                                      UseOnlyPtr<Bits> live_docs) {
  if (auto bulk_scorer = get_boolean_scorer(leaf_reader_context, leaf_collector, live_docs)) {
    return bulk_scorer;
  } else {
    return Weight::get_bulk_scorer(leaf_reader_context, leaf_collector, live_docs);
  }
}

ManagedPtr<Scorer> BooleanWeight::get_scorer(UseOnlyPtr<LeafReaderContext> leaf_reader_context) {
  if (auto scorer_supplier = get_scorer_supplier(leaf_reader_context)) {
    return scorer_supplier->get(std::numeric_limits<int64_t>::max());
  }
  return nullptr;
}

ManagedPtr<ScorerSupplier> BooleanWeight::get_scorer_supplier(UseOnlyPtr<LeafReaderContext> leaf_reader_context) {
  int32_t min_should_match = boolean_query->minimum_number_should_match;

  OccurEnumMap<LVector<ManagedPtr<ScorerSupplier>>> scorers{
      LinearAllocatorAdapterForSTL<ManagedPtr<ScorerSupplier>>(srch_context->linear_allocator)};

  for (auto &wc : weighted_clauses) {
    auto sub_scorer = wc.weight->get_scorer_supplier(leaf_reader_context);
    if (sub_scorer) {
      scorers[wc.occur].emplace_back(sub_scorer);
    } else if (wc.is_required()) {
      return nullptr;
    }
  }  // End for

  // get_scorer simplifications:
  if (scorers[BooleanClause::Occur::SHOULD].size() == min_should_match) {
    // any optional clauses are in fact required
    auto &should_scorers = scorers[BooleanClause::Occur::SHOULD];
    auto &must_scorers = scorers[BooleanClause::Occur::MUST];
    must_scorers.insert(must_scorers.end(), should_scorers.begin(), should_scorers.end());
    should_scorers.clear();
    min_should_match = 0;
  }

  if (scorers[BooleanClause::Occur::FILTER].empty()
      && scorers[BooleanClause::Occur::MUST].empty()
      && scorers[BooleanClause::Occur::SHOULD].empty()) {
    // no required and optional clauses.
    return nullptr;
  } else if (scorers[BooleanClause::Occur::SHOULD].size() < min_should_match) {
    // either > 1 req get_scorer, or there are 0 req scorers and at least 1
    // optional get_scorer. Therefore, if there are not enough optional scorers
    // no documents will be matched by the query
    return nullptr;
  }

  if (!score_mode_need_scores(score_mode)
      && min_should_match == 0
      && (scorers[BooleanClause::Occur::MUST].size() + scorers[BooleanClause::Occur::FILTER].size()) > 0) {
    // Purely optional clauses are useless without scoring.
    scorers[BooleanClause::Occur::SHOULD].clear();
  }

  return srch_context->linear_allocator->allocate_object<Boolean2ScorerSupplier>(
      this, std::move(scorers), score_mode, min_should_match);
}

ManagedPtr<BulkScorer> BooleanWeight::get_boolean_scorer(
    UseOnlyPtr<LeafReaderContext> leaf_reader_context,
    ManagedPtr<LeafCollector> leaf_collector,
    ManagedPtr<Bits> live_docs) {
  const int32_t num_optional_clauses = boolean_query->clause_sets[BooleanClause::Occur::SHOULD].size();
  const int32_t num_required_clauses =
      boolean_query->clause_sets[BooleanClause::Occur::MUST].size()
          + boolean_query->clause_sets[BooleanClause::Occur::FILTER].size();

  ManagedPtr<BulkScorer> positive_scorer = nullptr;
  if (num_required_clauses == 0) {
    if (positive_scorer = optional_bulk_scorer(leaf_reader_context, leaf_collector, live_docs);
        positive_scorer == nullptr) {
      return nullptr;
    }

    // TODO: what is the right heuristic here?
    int64_t cost_threshold;
    if (boolean_query->minimum_number_should_match <= 1) {
      // when all clauses are optional, use BooleanScorer aggressively
      // TODO: is there actually a threshold under which we should rather
      // use the regular get_scorer?
      cost_threshold = -1;
    } else {
      // when a minimum number of clauses should match, BooleanScorer is
      // going to score all windows that have at least minimum_number_should_match
      // matches in the window. But there is no way to know if there is
      // an intersection (all clauses might match a different doc ID and
      // there will be no matches in the end) so we should only use
      // BooleanScorer if matches are very dense
      cost_threshold = leaf_reader_context->segment_reader->get_max_doc() / 3;
    }

    if (positive_scorer->get_cost() < cost_threshold) {
      return nullptr;
    }
  } else if (num_required_clauses == 1
      && num_optional_clauses == 0
      && boolean_query->minimum_number_should_match == 0) {
    // TODO: there are some cases where BooleanScorer would handle conjunctions faster than BooleanScorer2...??
    positive_scorer = required_bulk_scorer(leaf_reader_context, leaf_collector, live_docs);
  } else {
    return nullptr;
  }  // End if

  if (positive_scorer == nullptr) {
    return nullptr;
  }

  LVector<ManagedPtr<Scorer>> prohibited{
      LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{srch_context->linear_allocator}};
  prohibited.reserve(boolean_query->clause_sets[BooleanClause::Occur::MUST_NOT].size());

  for (auto &wc : weighted_clauses) {
    if (wc.is_prohibited()) {
      if (auto scorer = wc.weight->get_scorer(leaf_reader_context)) {
        prohibited.emplace_back(scorer);
      }
    }
  }  // End for

  if (prohibited.empty()) {
    return positive_scorer;
  } else {
    auto prohibited_scorer =
        prohibited.size() == 1 ? prohibited[0]
                               : srch_context->linear_allocator->allocate_object<DisjunctionSumScorer>(
            this, std::move(prohibited), ScoreMode::COMPETE_NO_SCORES);
    if (prohibited_scorer->two_phase_iterator()) {
      // ReqExclBulkScorer can't deal efficiently with two-phased prohibited clauses
      return nullptr;
    }

    return srch_context->linear_allocator->allocate_object<BooleanReqExclBulkScorer>(
        positive_scorer,
        prohibited_scorer->get_iterator());
  }  // End if
}

ManagedPtr<BulkScorer> BooleanWeight::required_bulk_scorer(UseOnlyPtr<LeafReaderContext> leaf_reader_context,
                                                           ManagedPtr<LeafCollector> leaf_collector,
                                                           UseOnlyPtr<Bits> live_docs) {
  ManagedPtr<BulkScorer> scorer = nullptr;

  for (auto &wc : weighted_clauses) {
    if (!wc.is_required()) {
      continue;
    }
    if (scorer) {
      // we don't have a BulkScorer for conjunctions
      return nullptr;
    }

    scorer = wc.weight->get_bulk_scorer(leaf_reader_context, leaf_collector, live_docs);
    if (scorer == nullptr) {
      // no matches
      return nullptr;
    }
  }  // End for

  return scorer;
}

ManagedPtr<BulkScorer> BooleanWeight::optional_bulk_scorer(UseOnlyPtr<LeafReaderContext> leaf_reader_context,
                                                           ManagedPtr<LeafCollector> leaf_collector,
                                                           ManagedPtr<Bits> live_docs) {
  if (score_mode == ScoreMode::TOP_SCORES) {
    if (!boolean_query->is_pure_disjunction()) {
      return nullptr;
    }

    LVector<ManagedPtr<ScorerSupplier>> optional{LinearAllocatorAdapterForSTL<ManagedPtr<ScorerSupplier>>{
        srch_context->linear_allocator}};
    optional.reserve(boolean_query->clause_sets[BooleanClause::Occur::SHOULD].size());

    for (auto &wc : weighted_clauses) {
      if (wc.occur != BooleanClause::Occur::SHOULD) {
        continue;
      }
      auto opt_scorer_supplier = wc.weight->get_scorer_supplier(leaf_reader_context);
      if (opt_scorer_supplier) {
        optional.emplace_back(opt_scorer_supplier);
      }
    }

    if (optional.size() <= 1) {
      return nullptr;
    }

    LVector<ManagedPtr<Scorer>> optional_scorers{LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{
        srch_context->linear_allocator}};
    optional_scorers.resize(optional.size());
    for (int32_t i = 0; i < optional.size(); ++i) {
      optional_scorers[i] = optional[i]->get(std::numeric_limits<int64_t>::max());
    }

    return srch_context->linear_allocator->allocate_object<MaxScoreBulkScorer>(
        srch_context->linear_allocator,
        leaf_reader_context->segment_reader->get_max_doc(),
        std::move(optional_scorers),
        leaf_collector,
        live_docs);
  }  // End if

  LVector<ManagedPtr<BulkScorer>> optional{LinearAllocatorAdapterForSTL<ManagedPtr<BulkScorer>>{
      srch_context->linear_allocator}};
  optional.reserve(boolean_query->clause_sets[BooleanClause::Occur::SHOULD].size());

  for (auto &wc : weighted_clauses) {
    if (wc.occur != BooleanClause::Occur::SHOULD) {
      continue;
    }

    if (auto sub_scorer = wc.weight->get_bulk_scorer(leaf_reader_context, leaf_collector, live_docs)) {
      optional.emplace_back(sub_scorer);
    }
  }  // End for

  if (optional.empty()) {
    return nullptr;
  }

  if (boolean_query->minimum_number_should_match > optional.size()) {
    return nullptr;
  }

  if (optional.size() == 1) {
    return optional[0];
  }

  return srch_context->linear_allocator->allocate_object<BooleanScorer>(
      leaf_collector,
      this,
      std::move(optional),
      std::max(1, boolean_query->minimum_number_should_match));
}
