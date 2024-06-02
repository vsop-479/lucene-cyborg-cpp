#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_TWOPHASEITERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_TWOPHASEITERATOR_HPP_

#include "../util/PointerTypes.hpp"
#include "../util/LinearAllocator.hpp"
#include "../search/DocIdSetIterator.hpp"

namespace lucene::cyborg::search {

struct TwoPhaseIterator {
  static const util::TypeInfoPtr TYPE_INFO;

  static util::ManagedPtr<DocIdSetIterator> as_doc_id_set_iterator(
      util::ManagedPtr<TwoPhaseIterator> two_phase_iterator,
      util::UseOnlyPtr<util::LinearAllocator> allocator);

  static util::ManagedPtr<TwoPhaseIterator> unwrap(util::ManagedPtr<DocIdSetIterator> iterator);

  explicit TwoPhaseIterator(
      util::ManagedPtr<DocIdSetIterator> approximation,
      util::TypeInfoPtr two_phase_iterator_type_info = TYPE_INFO);

  virtual bool matches() = 0;

  virtual float get_match_cost() = 0;

  util::ManagedPtr<DocIdSetIterator> approximation;
  util::TypeInfoPtr two_phase_iterator_type_info;
};  // TwoPhaseIterator



struct TwoPhaseIteratorAsDocIdSetIterator final : public DocIdSetIterator {
  static const util::TypeInfoPtr TYPE_INFO;

  explicit TwoPhaseIteratorAsDocIdSetIterator(util::ManagedPtr<TwoPhaseIterator> _two_phase_iterator)
      : DocIdSetIterator(TYPE_INFO),
        two_phase_iterator(_two_phase_iterator),
        approximation(_two_phase_iterator->approximation) {
  }

  int64_t get_cost() final {
    return approximation->get_cost();
  }

  int32_t advance(int32_t target) final {
    for (int32_t doc = target;; doc = approximation->next_doc()) {
      if (doc != NO_MORE_DOCS) {
        if (two_phase_iterator->matches()) {
          return doc;
        }
      } else {
        return NO_MORE_DOCS;
      }
    }  // End for
  }

  int32_t get_doc_id() final {
    return approximation->get_doc_id();
  }

  int32_t next_doc() final {
    for (int32_t doc = approximation->next_doc();; doc = approximation->next_doc()) {
      if (doc != NO_MORE_DOCS) {
        if (two_phase_iterator->matches()) {
          return doc;
        }
      } else {
        return NO_MORE_DOCS;
      }
    }  // End for
  }

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator;
  util::ManagedPtr<DocIdSetIterator> approximation;
};  // TwoPhaseIteratorAsDocIdSetIterator



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_TWOPHASEITERATOR_HPP_
