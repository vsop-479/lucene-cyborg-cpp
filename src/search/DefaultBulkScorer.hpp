#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORER_HPP_

#include "BulkScorer.hpp"
#include "Scorer.hpp"
#include "../util/Bits.hpp"

namespace lucene::cyborg::search {

struct DefaultBulkScorer final : public BulkScorer {
  DefaultBulkScorer(
      util::ManagedPtr<Scorer> _scorer,
      util::ManagedPtr<DocIdSetIterator> _iterator,
      util::ManagedPtr<TwoPhaseIterator> _two_phase_iterator,
      util::ManagedPtr<LeafCollector> _leaf_collector,
      util::ManagedPtr<util::Bits> _live_docs
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
    auto scorer_iterator = two_phase == nullptr ? iterator : two_phase->approximation;

    if (min_doc_id == 0
        && max_doc_id == DocIdSetIterator::NO_MORE_DOCS
        && scorer_iterator->get_doc_id() == DocIdSetIterator::ADVANCE_NOT_YET_CALLED) {
      score_all(scorer_iterator);
      return DocIdSetIterator::NO_MORE_DOCS;
    } else {
      return score_range(scorer_iterator, min_doc_id, max_doc_id);
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
  void score_all(util::UseOnlyPtr<DocIdSetIterator> scorer_iterator) {
    if (two_phase == nullptr) {
      if (live_docs) {
        for (int32_t doc = scorer_iterator->next_doc(); live_docs->get(doc) && doc != DocIdSetIterator::NO_MORE_DOCS;
             doc = scorer_iterator->next_doc()) {
          leaf_collector->collect_one_doc(doc);
        }
      } else {
        for (int32_t doc = scorer_iterator->next_doc(); doc != DocIdSetIterator::NO_MORE_DOCS;
             doc = scorer_iterator->next_doc()) {
          leaf_collector->collect_one_doc(doc);
        }
      }
    } else {
      if (live_docs) {
        for (int32_t doc = scorer_iterator->next_doc(); doc != DocIdSetIterator::NO_MORE_DOCS;
             doc = scorer_iterator->next_doc()) {
          if (live_docs->get(doc) && two_phase->matches()) {
            leaf_collector->collect_one_doc(doc);
          }
        }
      } else {
        for (int32_t doc = scorer_iterator->next_doc(); doc != DocIdSetIterator::NO_MORE_DOCS;
             doc = scorer_iterator->next_doc()) {
          if (two_phase->matches()) {
            leaf_collector->collect_one_doc(doc);
          }
        }
      }
    }  // End if
  }

  int32_t score_range(util::UseOnlyPtr<DocIdSetIterator> scorer_iterator,
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

    if (two_phase == nullptr) {
      if (live_docs) {
        while (doc < max_doc) {
          if (live_docs->get(doc)) {
            leaf_collector->collect_one_doc(doc);
          }
          doc = iterator->next_doc();
        }  // End while
      } else {
        while (doc < max_doc) {
          leaf_collector->collect_one_doc(doc);
          doc = iterator->next_doc();
        }  // End while
      }
    } else {
      if (live_docs) {
        while (doc < max_doc) {
          if (two_phase->matches() && live_docs->get(doc)) {
            leaf_collector->collect_one_doc(doc);
          }
          doc = iterator->next_doc();
        }  // End while
      } else {
        while (doc < max_doc) {
          if (two_phase->matches()) {
            leaf_collector->collect_one_doc(doc);
          }
          doc = iterator->next_doc();
        }  // End while
      }
    }  // End if

    return doc;
  }

  util::ManagedPtr<Scorer> scorer;
  util::ManagedPtr<DocIdSetIterator> iterator;
  util::ManagedPtr<TwoPhaseIterator> two_phase;
  util::ManagedPtr<LeafCollector> leaf_collector;
  util::ManagedPtr<util::Bits> live_docs;
};  // DefaultBulkScorer



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DEFAULTBULKSCORER_HPP_
