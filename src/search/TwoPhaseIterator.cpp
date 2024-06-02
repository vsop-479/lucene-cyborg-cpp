#include "TwoPhaseIterator.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::TwoPhaseIteratorAsDocIdSetIterator;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// TwoPhaseIterator
//

const TypeInfoPtr TwoPhaseIterator::TYPE_INFO =
    TypeInfo::get_type_info<TwoPhaseIterator>("TwoPhaseIterator");

TwoPhaseIterator::TwoPhaseIterator(ManagedPtr<DocIdSetIterator> _approximation,
                                   TypeInfoPtr _two_phase_iterator_type_info)
    : approximation(_approximation),
      two_phase_iterator_type_info(_two_phase_iterator_type_info) {
}

ManagedPtr<DocIdSetIterator> TwoPhaseIterator::as_doc_id_set_iterator(
    ManagedPtr<TwoPhaseIterator> two_phase_iterator,
    UseOnlyPtr<LinearAllocator> allocator) {
  return allocator->allocate_object<TwoPhaseIteratorAsDocIdSetIterator>(two_phase_iterator);
}

ManagedPtr<TwoPhaseIterator> TwoPhaseIterator::unwrap(
    ManagedPtr<DocIdSetIterator> iterator) {
  if (iterator->get_type_info() == TwoPhaseIteratorAsDocIdSetIterator::TYPE_INFO) {
    return ((TwoPhaseIteratorAsDocIdSetIterator *) iterator)->two_phase_iterator;
  } else {
    return nullptr;
  }
}

//
// TwoPhaseIteratorAsDocIdSetIterator
//

const TypeInfoPtr TwoPhaseIteratorAsDocIdSetIterator::TYPE_INFO =
    TypeInfo::get_type_info<TwoPhaseIteratorAsDocIdSetIterator>("TwoPhaseIteratorAsDocIdSetIterator");
