#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_HITQUEUE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_HITQUEUE_HPP_

#include "../util/PriorityQueue.hpp"
#include "ScoreDoc.hpp"

namespace lucene::cyborg::search {

class HitQueue final : public util::PriorityQueue<util::ManagedPtr<ScoreDoc>, HitQueue> {
 public:
  bool less_than(util::ManagedPtr<ScoreDoc> sd0, util::ManagedPtr<ScoreDoc> sd1) const noexcept {
    if (sd0->score != sd1->score) {
      return sd0->score < sd1->score;
    } else {
      return sd0->doc > sd1->doc;
    }
  }
};  // HitQueue



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_HITQUEUE_HPP_
