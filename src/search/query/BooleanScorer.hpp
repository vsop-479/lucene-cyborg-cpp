#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANSCORER_HPP_

#include "../BulkScorer.hpp"
#include "../../util/Bits.hpp"
#include "../../util/Macros.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../util/PriorityQueue.hpp"
#include "BooleanWeight.hpp"
#include "../ScoreAndDoc.hpp"

namespace lucene::cyborg::search::query {

static constexpr uint32_t BS_SHIFT = 11U;
static constexpr int32_t BS_BUCKET_LIST_SIZE = 1U << BS_SHIFT;
static constexpr int32_t BS_INDICES_SET_SIZE = 1 << (BS_SHIFT - 6);
static constexpr uint32_t BS_MASK = BS_BUCKET_LIST_SIZE - 1;

struct BooleanScorerOrCollector;

struct BooleanScorerOrCollectorReplayerBase {
  virtual void for_each() = 0;
};  // BooleanScorerOrCollectorReplayerBase

template<typename DocIdConsumerType>
struct BooleanScorerOrCollectorReplayer final : public BooleanScorerOrCollectorReplayerBase {
  BooleanScorerOrCollectorReplayer(util::ManagedPtr<DocIdConsumerType> _doc_id_consumer,
                                   util::ManagedPtr<BooleanScorerOrCollector> _or_collector)
      : doc_id_consumer(_doc_id_consumer),
        or_collector(_or_collector) {
  }

  void for_each() final;

  util::ManagedPtr<DocIdConsumerType> doc_id_consumer;
  util::ManagedPtr<BooleanScorerOrCollector> or_collector;
};  // BooleanScorerOrCollectorReplayer


struct BooleanScorerOrCollector final : public LeafCollector, public DocIdStream {
  template<typename DocIdConsumerType>
  friend
  class BooleanScorerOrCollectorReplayer;

  struct Bucket {
    double score;
    int32_t freq;
    int32_t doc_id;
  };  // Bucket

  BooleanScorerOrCollector(util::ManagedPtr<SearchContext> srch_context,
                           util::ManagedPtr<LeafCollector> parent_collector,
                           bool needs_score,
                           int32_t min_should_match);

  void collect_one_doc(int32_t doc_id) final;

  void prepare_collecting_multiple_sources(int32_t base_doc_id);

  void for_each(util::UseOnlyPtr<DocIdConsumer> doc_id_consumer) final;

  int32_t count() final;

  util::ManagedPtr<LeafCollector> parent_collector;
  util::ManagedPtr<SearchContext> srch_context;
  util::TypedVector<Bucket> buckets;
  util::TypedVector<uint64_t> valid_indices;
  ScoreAndDoc score_and_doc;
  bool needs_score;
  int32_t min_should_match;
  int32_t base_doc_id;
};  // OrCollector

class BooleanScorer final : public BulkScorer {
 private:
  struct BulkScorerAndDoc {
    explicit BulkScorerAndDoc(util::ManagedPtr<BulkScorer> _scorer)
        : scorer(_scorer),
          cost(_scorer->get_cost()),
          next(-1) {
    }

    LC_ALWAYSINLINE void advance(int32_t min_doc_id) {
      score(min_doc_id, min_doc_id);
    }

    LC_ALWAYSINLINE void score(int32_t min_doc_id,
                               int32_t max_doc_id) {
      next = scorer->score(min_doc_id, max_doc_id);
    }

    util::ManagedPtr<BulkScorer> scorer;
    int64_t cost;
    int32_t next;
  };  // BulkScorerAndDoc

  struct HeadPriorityQueue final : public util::PriorityQueue<util::ManagedPtr<BulkScorerAndDoc>, HeadPriorityQueue> {
    bool less_than(util::ManagedPtr<BulkScorerAndDoc> a, util::ManagedPtr<BulkScorerAndDoc> b) noexcept {
      return a->next < b->next;
    }
  };  // HeadPriorityQueue

  struct TailPriorityQueue final : public util::PriorityQueue<util::ManagedPtr<BulkScorerAndDoc>, TailPriorityQueue> {
    bool less_than(util::ManagedPtr<BulkScorerAndDoc> a, util::ManagedPtr<BulkScorerAndDoc> b) noexcept {
      return a->cost < b->cost;
    }
  };  // HeadPriorityQueue

  static int64_t get_cost(util::ManagedPtr<util::LinearAllocator> allocator,
                          util::LVector<util::ManagedPtr<BulkScorer>> &scorers,
                          int32_t min_should_match);

  util::ManagedPtr<BooleanScorer::BulkScorerAndDoc> advance_sub_bulk_scorers(int32_t min_doc_id);

  util::ManagedPtr<BulkScorerAndDoc> score_window(util::ManagedPtr<BulkScorerAndDoc> top,
                                                  int32_t min_doc_id,
                                                  int32_t max_doc_id);

  void score_window_multiple_scorers(int32_t window_min, int32_t window_max, int32_t max_freq);

  void score_window_single_scorer(util::ManagedPtr<BulkScorerAndDoc> bulk_scorer,
                                  int32_t window_min,
                                  int32_t window_max,
                                  int32_t max_doc_id);

  void score_window_into_bit_set_and_replay(int32_t min_doc_id, int32_t max_doc_id, int32_t num_scorers);

 public:
  BooleanScorer(util::ManagedPtr<LeafCollector> leaf_collector,
                util::ManagedPtr<BooleanWeight> weight,
                util::LVector<util::ManagedPtr<BulkScorer>> &&scorers,
                int32_t min_should_match);

  int64_t get_cost() final {
    return cost;
  }

  util::ManagedPtr<LeafCollector> get_collector() final {
    return leaf_collector;
  }

  util::ManagedPtr<LeafCollector> reset_collector(util::ManagedPtr<LeafCollector> new_collector) final {
    std::swap(leaf_collector, new_collector);
    return new_collector;
  }

  int32_t score(int32_t min_doc_id, int32_t max_doc_id) final;

 private:
  util::ManagedPtr<LeafCollector> leaf_collector;
  int64_t cost;
  util::TypedVector<util::ManagedPtr<BulkScorerAndDoc>> leads;
  HeadPriorityQueue head;
  TailPriorityQueue tail;
  BooleanScorerOrCollector or_collector;
  int32_t min_should_match;
};  // BooleanScorer


template<typename DocIdConsumerType>
void BooleanScorerOrCollectorReplayer<DocIdConsumerType>::for_each() {
  // Use valid indices bit set + buckets.
  const bool has_bucket = or_collector->buckets.vec;
  for (int32_t i = 0; i < BS_INDICES_SET_SIZE; ++i) {
    if (uint64_t bits = or_collector->valid_indices.vec[i]) {
      const int32_t i64 = i * 64;
      do {
        const int32_t ntz = std::countr_zero(bits);
        if (has_bucket) {
          const int32_t bucket_index = i64 + ntz;
          or_collector->score_and_doc.score = (float) or_collector->buckets.vec[bucket_index].score;
          or_collector->score_and_doc.doc = or_collector->buckets.vec[bucket_index].doc_id;
          doc_id_consumer->collect_one_doc(or_collector->score_and_doc.doc);
          or_collector->buckets.vec[bucket_index].score = 0;
          or_collector->buckets.vec[bucket_index].freq = 0;
          bits ^= 1ULL << ntz;
        } else {
          const int32_t doc_id = or_collector->base_doc_id | (i << 6U) | ntz;
          or_collector->score_and_doc.doc = doc_id;
          doc_id_consumer->collect_one_doc(doc_id);
          bits ^= 1ULL << ntz;
        }
      } while (bits);
    }  // End if
  }  // End for
}

}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANSCORER_HPP_
