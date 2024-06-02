#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORERPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORERPCO_HPP_

#include "BulkScorer.hpp"
#include "Scorer.hpp"
#include "../util/Bits.hpp"
#include "../util/Macros.hpp"
#include "../pco/util/PcoNullType.hpp"

namespace lucene::cyborg::search {

template<typename DocIdSetIteratorType, typename TwoPhaseIteratorType, typename BitsType>
class DefaultBulkScorerPco final : public BulkScorer {
  static constexpr bool IS_TWO_PHASE_NULL = pco::PcoNullType::is_null_type<TwoPhaseIteratorType>();
  static constexpr bool IS_BITS_NULL = pco::PcoNullType::is_null_type<BitsType>();

 public:
  DefaultBulkScorerPco(
      util::ManagedPtr<DocIdSetIteratorType> _iterator,
      util::ManagedPtr<TwoPhaseIteratorType> _two_phase_iterator,
      util::ManagedPtr<BitsType> _live_docs,
      util::ManagedPtr<Scorer> _scorer,
      util::ManagedPtr<LeafCollector> _leaf_collector
  ) : BulkScorer(),
      scorer(_scorer),
      iterator(_iterator),
      two_phase(_two_phase_iterator),
      leaf_collector(_leaf_collector),
      live_docs(_live_docs) {
    if (_scorer == nullptr) [[unlikely]] {
      // TODO : NPE
      throw 13;
    }
    if (_leaf_collector == nullptr) [[unlikely]] {
      // TODO : NPE
      throw 13;
    }
  }

  int32_t score(int32_t min_doc_id, int32_t max_doc_id) final {
    leaf_collector->set_scorer(scorer);

    if (min_doc_id == 0
        && max_doc_id == DocIdSetIterator::NO_MORE_DOCS) {
      if constexpr (IS_TWO_PHASE_NULL) {
        if (iterator->get_doc_id() == DocIdSetIterator::ADVANCE_NOT_YET_CALLED) {
          score_all<DocIdSetIteratorType>(iterator);
          return DocIdSetIterator::NO_MORE_DOCS;
        }
      } else {
        if (two_phase->approximation->get_doc_id() == DocIdSetIterator::ADVANCE_NOT_YET_CALLED) {
          // TODO
          throw 13;
        }
      }
    }  // End if

    if constexpr (IS_TWO_PHASE_NULL) {
      return score_range<DocIdSetIteratorType>(iterator, min_doc_id, max_doc_id);
    } else {
      // TODO
      throw 13;
    }  // End if
  }

  util::ManagedPtr<LeafCollector> get_collector() final {
    return leaf_collector;
  }

  util::ManagedPtr<LeafCollector> reset_collector(util::ManagedPtr<LeafCollector> new_collector) final {
    std::swap(leaf_collector, new_collector);
    return new_collector;
  }

  int64_t get_cost() final {
    return iterator->get_cost();
  }

 private:
  LC_ALWAYSINLINE bool is_live_doc(int32_t doc) {
    if constexpr (IS_BITS_NULL) {
      return true;
    } else {
      return live_docs->get(doc);
    }
  }

  template<typename ActualDocIdSetIterator>
  void score_all(util::UseOnlyPtr<ActualDocIdSetIterator> scorer_iterator) {
    if constexpr (IS_TWO_PHASE_NULL) {
      for (int32_t doc = scorer_iterator->next_doc();
           is_live_doc(doc) && doc != DocIdSetIterator::NO_MORE_DOCS;
           doc = scorer_iterator->next_doc()) {
        leaf_collector->collect_one_doc(doc);
      }
    } else {
      for (int32_t doc = scorer_iterator->next_doc(); doc != DocIdSetIterator::NO_MORE_DOCS;
           doc = scorer_iterator->next_doc()) {
        if (is_live_doc(doc) && two_phase->matches()) {
          leaf_collector->collect_one_doc(doc);
        }
      }
    }  // End if
  }

  template<typename ActualDocIdSetIterator>
  int32_t score_range(util::UseOnlyPtr<ActualDocIdSetIterator> scorer_iterator,
                      const int32_t min_doc,
                      const int32_t max_doc) {
    int32_t doc = scorer_iterator->get_doc_id();
    if (doc < min_doc) {
      if (doc == min_doc - 1) {
        doc = scorer_iterator->next_doc();
      } else {
        doc = scorer_iterator->advance(min_doc);
      }
    }  // End if

    if constexpr (IS_TWO_PHASE_NULL) {
      while (doc < max_doc) {
        if (is_live_doc(doc)) {
          leaf_collector->collect_one_doc(doc);
        }

        doc = iterator->next_doc();
      }  // End while
    } else {
      while (doc < max_doc) {
        if (two_phase->matches()) {
          if (is_live_doc(doc)) {
            leaf_collector->collect_one_doc(doc);
          }
        }

        doc = iterator->next_doc();
      }  // End while
    }  // End if

    return doc;
  }

  util::ManagedPtr<Scorer> scorer;
  util::ManagedPtr<DocIdSetIteratorType> iterator;
  util::ManagedPtr<TwoPhaseIteratorType> two_phase;
  util::ManagedPtr<LeafCollector> leaf_collector;
  util::ManagedPtr<BitsType> live_docs;
};  // DefaultBulkScorerPco



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORERPCO_HPP_
