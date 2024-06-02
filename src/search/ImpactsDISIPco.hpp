#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISIPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISIPCO_HPP_

#include "../util/PointerTypes.hpp"
#include "DocIdSetIterator.hpp"
#include "MaxScoreCache.hpp"

namespace lucene::cyborg::search {

struct ImpactsDISIPcoBase : public DocIdSetIterator {
  ImpactsDISIPcoBase()
      : DocIdSetIterator(),
        min_competitive_score(),
        upto(DocIdSetIterator::NO_MORE_DOCS) {
  }

  void set_min_competitive_score(float _min_competitive_score) {
    assert(_min_competitive_score >= min_competitive_score);
    if (_min_competitive_score > min_competitive_score) {
      min_competitive_score = _min_competitive_score;
      // force upto and max_score to be recomputed so that we will skip documents
      // if the current block of documents is not competitive - only if the min
      // competitive score actually increased
      upto = -1;
    }
  }

  float min_competitive_score;
  int32_t upto;
};  // ImpactsDISIPcoBase



template<typename DocIdSetIteratorType>
class ImpactsDISIPco final : public ImpactsDISIPcoBase {
 public:
  static uint64_t PCO_INDEX;

  ImpactsDISIPco(util::ManagedPtr<DocIdSetIteratorType> _in,
                 util::ManagedPtr<MaxScoreCache> _max_score_cache)
      : ImpactsDISIPcoBase(),
        in(_in),
        max_score_cache(_max_score_cache),
        max_score(std::numeric_limits<float>::max()) {
    pco_type_index = PCO_INDEX;
  }

  int32_t advance(int32_t target) final {
    return in->advance(advance_target(target));
  }

  int32_t next_doc() final {
    const int32_t next_doc = in->get_doc_id() + 1;
    if (next_doc <= upto) {
      return in->advance(next_doc);
    } else {
      return in->advance(advance_target(next_doc));
    }
  }

  int32_t get_doc_id() final {
    return in->get_doc_id();
  }

  int64_t get_cost() final {
    return in->get_cost();
  }

 private:
  int32_t advance_target(int32_t target) {
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

  util::ManagedPtr<DocIdSetIteratorType> in;
  util::ManagedPtr<MaxScoreCache> max_score_cache;
  float max_score;
};  // ImpactsDISI



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISIPCO_HPP_
