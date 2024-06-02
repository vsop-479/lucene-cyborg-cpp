#include <limits>
#include "Boolean2ScorerSupplier.hpp"
#include "../ScorerUtil.hpp"
#include "ConjunctionScorer.hpp"
#include "ReqOptSumScorer.hpp"
#include "DisjunctionSumScorer.hpp"
#include "BooleanReqExclScorer.hpp"
#include "BlockMaxConjunctionScorer.hpp"

using lucene::cyborg::search::BlockMaxConjunctionScorer;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::ScorerSupplier;
using lucene::cyborg::search::ScorerUtil;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::Boolean2ScorerSupplier;
using lucene::cyborg::search::query::BooleanReqExclScorer;
using lucene::cyborg::search::query::ConjunctionScorer;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;

//
// Boolean2ScorerSupplier
//

Boolean2ScorerSupplier::Boolean2ScorerSupplier(ManagedPtr<Weight> _weight,
                                               OccurEnumMap<LVector<ManagedPtr<ScorerSupplier>>> &&_scorer_suppliers,
                                               ScoreMode _score_mode,
                                               int32_t _min_should_match)
    : ScorerSupplier(),
      weight(_weight),
      scorer_suppliers(std::move(_scorer_suppliers)),
      score_mode(_score_mode),
      min_should_match(_min_should_match),
      cost(-1),
      top_level_scoring_clause() {
  if (min_should_match < 0) [[unlikely]] {
    // TODO
    throw 13;
  }

  if (min_should_match != 0 && min_should_match >= scorer_suppliers[BooleanClause::Occur::SHOULD].size()) [[unlikely]] {
    // TODO
    throw 13;
  }

  if (!score_mode_need_scores(score_mode)
      && min_should_match == 0
      && !scorer_suppliers[BooleanClause::Occur::SHOULD].empty()
      && (scorer_suppliers[BooleanClause::Occur::MUST].size()
          + scorer_suppliers[BooleanClause::Occur::FILTER].size())) [[unlikely]] {
    // TODO
    throw 13;
  }

  if (scorer_suppliers[BooleanClause::Occur::SHOULD].empty()
      && scorer_suppliers[BooleanClause::Occur::MUST].empty()
      && scorer_suppliers[BooleanClause::Occur::FILTER].empty()) [[unlikely]] {
    // TODO
    throw 13;
  }
}

ManagedPtr<Scorer> Boolean2ScorerSupplier::get(int64_t lead_cost) {
  auto scorer = do_get_scorer(lead_cost);

  // TODO : Can we safely uncomment below??
  // if (score_mode == ScoreMode::TOP_SCORES
  //     && scorer_suppliers[BooleanClause::Occur::SHOULD].empty()
  //     && scorer_suppliers[BooleanClause::Occur::MUST].empty()) {
  //   // I mean... isn't a weight for 'filtering' was already configured 'not for scoring'??
  //   // why we need to wrap constant score scorer on top of that???
  //   return scorer;
  // }  // End if

  return scorer;
}

int64_t Boolean2ScorerSupplier::get_cost() {
  if (cost != -1) {
    return cost;
  } else {
    return cost = compute_cost();
  }
}

void Boolean2ScorerSupplier::set_top_level_scoring_clause() {
  top_level_scoring_clause = true;
  if (scorer_suppliers[BooleanClause::Occur::SHOULD].size()
      + scorer_suppliers[BooleanClause::Occur::MUST].size() == 1) {
    // If there is a single scoring clause, propagate the call.
    for (auto scorer_supplier : scorer_suppliers[BooleanClause::Occur::SHOULD]) {
      scorer_supplier->set_top_level_scoring_clause();
    }
    for (auto scorer_supplier : scorer_suppliers[BooleanClause::Occur::MUST]) {
      scorer_supplier->set_top_level_scoring_clause();
    }
  }  // End if
}

ManagedPtr<Scorer> Boolean2ScorerSupplier::do_get_scorer(int64_t lead_cost) {
  // three cases: conjunction, disjunction, or mixed
  lead_cost = std::min(lead_cost, get_cost());

  // pure conjunction
  if (scorer_suppliers[BooleanClause::Occur::SHOULD].empty()) {
    return exclude(
        require(scorer_suppliers[BooleanClause::FILTER],
                scorer_suppliers[BooleanClause::MUST],
                lead_cost, top_level_scoring_clause),
        scorer_suppliers[BooleanClause::MUST_NOT],
        lead_cost);
  }

  // pure disjunction
  if (scorer_suppliers[BooleanClause::Occur::FILTER].empty()
      && scorer_suppliers[BooleanClause::Occur::MUST].empty()) {
    return exclude(
        optional(scorer_suppliers[BooleanClause::Occur::SHOULD],
                 min_should_match,
                 score_mode,
                 lead_cost,
                 top_level_scoring_clause),
        scorer_suppliers[BooleanClause::Occur::MUST_NOT],
        lead_cost
    );
  }

  // conjunction-disjunction mix:
  // we create the required and optional pieces, and then
  // combine the two: if min_should_match > 0, then it's a conjunction: because the
  // optional side must match. otherwise it's required + optional

  if (min_should_match > 0) {
    auto req =
        exclude(
            require(scorer_suppliers[BooleanClause::Occur::FILTER],
                    scorer_suppliers[BooleanClause::Occur::MUST],
                    lead_cost, false),
            scorer_suppliers[BooleanClause::Occur::MUST_NOT],
            lead_cost);
    auto opt = optional(scorer_suppliers[BooleanClause::Occur::SHOULD],
                        min_should_match,
                        score_mode,
                        lead_cost,
                        false);
    LVector<ManagedPtr<Scorer>> req_opt_scorers{
        LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{weight->srch_context->linear_allocator}};
    req_opt_scorers.reserve(2);
    req_opt_scorers.emplace_back(req);
    req_opt_scorers.emplace_back(opt);
    LVector<ManagedPtr<Scorer>> req_opt_scorers1 = req_opt_scorers;

    return weight->srch_context->linear_allocator->allocate_object<ConjunctionScorer>(
        weight, std::move(req_opt_scorers), std::move(req_opt_scorers1));
  } else {
    assert(score_mode_need_scores(score_mode));
    return weight->srch_context->linear_allocator->allocate_object<ReqOptSumScorer>(
        exclude(
            require(scorer_suppliers[BooleanClause::Occur::FILTER],
                    scorer_suppliers[BooleanClause::Occur::MUST],
                    lead_cost, false),
            scorer_suppliers[BooleanClause::Occur::MUST_NOT],
            lead_cost),
        optional(scorer_suppliers[BooleanClause::Occur::SHOULD],
                 min_should_match,
                 score_mode,
                 lead_cost,
                 false),
        score_mode);
  }  // End if
}

ManagedPtr<Scorer> Boolean2ScorerSupplier::optional(
    LVector<ManagedPtr<ScorerSupplier>> &optional,
    int32_t min_should_match,
    ScoreMode score_mode,
    int64_t lead_cost,
    bool top_level_scoring_clause) {
  if (optional.size() == 1) {
    return optional[0]->get(lead_cost);
  } else {
    LVector<ManagedPtr<Scorer>> optional_scorers{
        LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{weight->srch_context->linear_allocator}};
    optional_scorers.reserve(optional.size());
    for (auto scorer : optional) {
      optional_scorers.emplace_back(scorer->get(lead_cost));
    }

    // Technically speaking, WANDScorer should be able to handle the following 3 conditions now
    // 1. Any ScoreMode (with scoring or not)
    // 2. Any min_competitive_score ( >= 0 )
    // 3. Any min_should_match ( >= 0 )
    //
    // However, as WANDScorer uses more complex algorithm and data structure, we would like to
    // still use DisjunctionSumScorer to handle exhaustive pure disjunctions, which may be faster
    if ((score_mode == ScoreMode::TOP_SCORES && top_level_scoring_clause) || min_should_match > 1) {
      // TODO
      assert(false);
    } else {
      return weight->srch_context->linear_allocator->allocate_object<DisjunctionSumScorer>(
          weight, std::move(optional_scorers), score_mode);
    }
  }  // End if
}

int64_t Boolean2ScorerSupplier::compute_cost() {
  int64_t min_required_cost = std::numeric_limits<int64_t>::max();

  for (auto scorer_supplier : scorer_suppliers[BooleanClause::Occur::MUST]) {
    if (auto cost = scorer_supplier->get_cost(); cost < min_required_cost) {
      min_required_cost = cost;
    }
  }
  for (auto scorer_supplier : scorer_suppliers[BooleanClause::Occur::FILTER]) {
    if (auto cost = scorer_supplier->get_cost(); cost < min_required_cost) {
      min_required_cost = cost;
    }
  }

  if (min_should_match == 0
      && (scorer_suppliers[BooleanClause::Occur::MUST].size()
          + scorer_suppliers[BooleanClause::Occur::FILTER].size())) {
    return min_required_cost;
  } else {
    LVector<int64_t> costs{LinearAllocatorAdapterForSTL<int64_t>{weight->srch_context->linear_allocator}};
    costs.reserve(scorer_suppliers[BooleanClause::Occur::SHOULD].size());
    for (auto scorer_supplier : scorer_suppliers[BooleanClause::Occur::SHOULD]) {
      costs.emplace_back(scorer_supplier->get_cost());
    }
    const int64_t should_cost = ScorerUtil::cost_with_min_should_match(
        costs.data(), (int32_t) costs.size(), min_should_match);
    return std::min(min_required_cost, should_cost);
  }  // End if
}

ManagedPtr<Scorer> Boolean2ScorerSupplier::exclude(ManagedPtr<Scorer> main,
                                                   LVector<ManagedPtr<ScorerSupplier>> &prohibited,
                                                   int64_t lead_cost) {
  if (prohibited.empty()) {
    return main;
  } else {
    return weight->srch_context->linear_allocator->allocate_object<BooleanReqExclScorer>(
        main,
        optional(prohibited, 1, ScoreMode::COMPETE_NO_SCORES, lead_cost, false));
  }  // End if
}

ManagedPtr<Scorer> Boolean2ScorerSupplier::require(LVector<ManagedPtr<ScorerSupplier>> &required_no_scoring,
                                                   LVector<ManagedPtr<ScorerSupplier>> &required_scoring,
                                                   int64_t lead_cost,
                                                   bool top_level_scoring_clause) {
  if ((required_no_scoring.size() + required_scoring.size()) == 1) {
    auto req = required_no_scoring.empty()
               ? required_scoring[0]->get(lead_cost) : required_no_scoring[0]->get(lead_cost);

    return req;
  } else {
    LVector<ManagedPtr<Scorer>> required_scorers{LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{
        weight->srch_context->linear_allocator}};
    required_scorers.reserve(required_no_scoring.size() + required_scoring.size());
    LVector<ManagedPtr<Scorer>> scoring_scorers{LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{
        weight->srch_context->linear_allocator}};
    scoring_scorers.reserve(required_scoring.size());

    for (auto scorer_supplier : required_no_scoring) {
      required_scorers.emplace_back(scorer_supplier->get(lead_cost));
    }
    for (auto scorer_supplier : required_scoring) {
      scoring_scorers.emplace_back(scorer_supplier->get(lead_cost));
    }

    if (score_mode == ScoreMode::TOP_SCORES && required_scoring.size() > 1 && top_level_scoring_clause) {
      auto block_max_scorer = weight->srch_context->linear_allocator->allocate_object<BlockMaxConjunctionScorer>(
          weight, std::move(scoring_scorers));
      if (required_scorers.empty()) {
        return block_max_scorer;
      }
      scoring_scorers = LVector<ManagedPtr<Scorer>>{LinearAllocatorAdapterForSTL<ManagedPtr<Scorer>>{}};
      scoring_scorers.emplace_back(block_max_scorer);
    }
    required_scorers.insert(required_scorers.end(), scoring_scorers.begin(), scoring_scorers.end());
    return weight->srch_context->linear_allocator->allocate_object<ConjunctionScorer>(
        weight, std::move(required_scorers), std::move(scoring_scorers));
  }  // End if
}
