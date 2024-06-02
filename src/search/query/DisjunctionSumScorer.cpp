#include "DisjunctionSumScorer.hpp"

using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::DisjunctionSumScorer;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;

//
// DisjunctionSumScorer
//

DisjunctionSumScorer::DisjunctionSumScorer(ManagedPtr<Weight> _weight,
                                           LVector<ManagedPtr<Scorer>> &&_sub_scorers,
                                           ScoreMode _score_mode)
    : DisjunctionScorer(_weight, std::move(_sub_scorers), _score_mode) {
}

float DisjunctionSumScorer::do_score(const TypedVector<ManagedPtr<DisiWrapper>> &sub_matches) {
  double score = 0;
  for (int32_t i = 0; i < sub_matches.len; ++i) {
    score += sub_matches.vec[i]->scorer->get_score();
  }
  return (float) score;
}
