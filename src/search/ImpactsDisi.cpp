#include <limits>
#include "ImpactsDisi.hpp"

using lucene::cyborg::search::ImpactsDISI;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::MaxScoreCache;

//
// ImpactsDISI
//

ImpactsDISI::ImpactsDISI(ManagedPtr<DocIdSetIterator> _in,
                         ManagedPtr<MaxScoreCache> _max_score_cache)
    : DocIdSetIterator(),
      in(_in),
      max_score_cache(_max_score_cache),
      min_competitive_score(),
      upto(DocIdSetIterator::NO_MORE_DOCS),
      max_score(std::numeric_limits<float>::max()) {
}

void ImpactsDISI::set_min_competitive_score(float _min_competitive_score) {
  assert(_min_competitive_score >= min_competitive_score);
  if (_min_competitive_score > min_competitive_score) {
    min_competitive_score = _min_competitive_score;
    // force upto and max_score to be recomputed so that we will skip documents
    // if the current block of documents is not competitive - only if the min
    // competitive score actually increased
    upto = -1;
  }
}

int32_t ImpactsDISI::advance(int32_t target) {
  return in->advance(advance_target(target));
}

int32_t ImpactsDISI::next_doc() {
  const int32_t next_doc = in->get_doc_id() + 1;
  if (next_doc <= upto) {
    return in->advance(next_doc);
  } else {
    return in->advance(advance_target(next_doc));
  }
}

int32_t ImpactsDISI::get_doc_id() {
  return in->get_doc_id();
}

int64_t ImpactsDISI::get_cost() {
  return in->get_cost();
}

int ImpactsDISI::advance_target(int32_t target) {
  if (target <= upto) {
    // we are still in the current block, which is considered competitive
    // according to impacts, no skipping
    return target;
  }

  while (true) {
    upto = max_score_cache->advance_shallow(target);
    max_score = max_score_cache->get_max_score_for_level(0);
    assert(upto >= target);

    if (max_score >= min_competitive_score) {
      return target;
    }

    if (upto == NO_MORE_DOCS) {
      return NO_MORE_DOCS;
    }

    int32_t skip_up_to = max_score_cache->get_skip_up_to(min_competitive_score);
    if (skip_up_to == -1) { // no further skipping
      target = upto + 1;
    } else if (skip_up_to == NO_MORE_DOCS) {
      return NO_MORE_DOCS;
    } else {
      target = skip_up_to + 1;
    }
  }  // End while
}
