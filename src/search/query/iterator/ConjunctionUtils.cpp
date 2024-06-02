#include <limits>
#include "ConjunctionUtils.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "BitSetIterator.hpp"
#include "ConjunctionDisi.hpp"
#include "ConjunctionDisiPco.hpp"
#include "ConjunctionTwoPhaseIterator.hpp"
#include "BitSetConjunctionDisi.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::query::BitSetConjunctionDISI;
using lucene::cyborg::search::query::BitSetIterator;
using lucene::cyborg::search::query::ConjunctionDISI;
using lucene::cyborg::search::query::ConjunctionTwoPhaseIterator;
using lucene::cyborg::search::query::ConjunctionUtils;
using lucene::cyborg::search::query::ConjunctionDISIPcoBase;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// Other functions
//

void add_two_phase_iterator(ManagedPtr<TwoPhaseIterator> two_phase_iterator,
                            LVector<ManagedPtr<DocIdSetIterator>> &iterators,
                            LVector<ManagedPtr<TwoPhaseIterator>> &two_phase_iterators);

void add_iterator(ManagedPtr<DocIdSetIterator> iterator,
                  LVector<ManagedPtr<DocIdSetIterator>> &iterators,
                  LVector<ManagedPtr<TwoPhaseIterator>> &two_phase_iterators) {
  auto two_phase = TwoPhaseIterator::unwrap(iterator);
  if (two_phase) {
    add_two_phase_iterator(two_phase, iterators, two_phase_iterators);
  } else if (iterator->get_type_info() == ConjunctionDISI::TYPE_INFO) { // Check for exactly this class for collapsing
    auto conjunction = (ConjunctionDISI *) iterator;
    // sub-conjuctions have already split themselves into two phase iterators and others, so we can
    // take those iterators as they are and move them up to this conjunction
    iterators.emplace_back(conjunction->lead1);
    iterators.emplace_back(conjunction->lead2);
    iterators.insert(iterators.end(), conjunction->others.begin(), conjunction->others.end());
  } else if (iterator->get_type_info() == BitSetConjunctionDISI::TYPE_INFO) {
    auto conjunction = (BitSetConjunctionDISI *) iterator;
    iterators.emplace_back(conjunction->lead);
    iterators.insert(iterators.end(), conjunction->bit_set_iterators.begin(), conjunction->bit_set_iterators.end());
  } else {
    iterators.emplace_back(iterator);
  }  // End if
}

void add_two_phase_iterator(ManagedPtr<TwoPhaseIterator> two_phase_iterator,
                            LVector<ManagedPtr<DocIdSetIterator>> &iterators,
                            LVector<ManagedPtr<TwoPhaseIterator>> &two_phase_iterators) {
  add_iterator(two_phase_iterator->approximation, iterators, two_phase_iterators);
  if (two_phase_iterator->two_phase_iterator_type_info
      == ConjunctionTwoPhaseIterator::TYPE_INFO) { // Check for exactly this class for collapsing
    two_phase_iterators.insert(two_phase_iterators.end(),
                               ((ConjunctionTwoPhaseIterator *) two_phase_iterator)->two_phase_iterators.begin(),
                               ((ConjunctionTwoPhaseIterator *) two_phase_iterator)->two_phase_iterators.end());
  } else {
    two_phase_iterators.emplace_back(two_phase_iterator);
  }
}

void add_scorer(UseOnlyPtr<Scorer> scorer,
                LVector<ManagedPtr<DocIdSetIterator>> &iterators,
                LVector<ManagedPtr<TwoPhaseIterator>> &two_phase_iterators) {
  if (auto two_phase_iterator = scorer->two_phase_iterator()) {
    add_two_phase_iterator(two_phase_iterator, iterators, two_phase_iterators);
  } else {
    add_iterator(scorer->get_iterator(), iterators, two_phase_iterators);
  }
}

ManagedPtr<DocIdSetIterator> create_conjunction(LVector<ManagedPtr<DocIdSetIterator>> &iterators,
                                                LVector<ManagedPtr<TwoPhaseIterator>> &two_phase_iterators,
                                                UseOnlyPtr<LinearAllocator> allocator) {
  // Check that all sub-iterators are on the same doc ID
  const int32_t cur_doc =
      iterators.size() ? iterators[0]->get_doc_id() : two_phase_iterators[0]->approximation->get_doc_id();
  int64_t min_cost = std::numeric_limits<int64_t>::max();
  for (auto iterator : iterators) {
    if (iterator->get_doc_id() == cur_doc) {
      min_cost = std::min(iterator->get_cost(), min_cost);
    } else {
      // TODO
      throw 13;
    }
  }  // End for
  for (auto two_phase_iterator : two_phase_iterators) {
    if (two_phase_iterator->approximation->get_doc_id() != cur_doc) {
      // TODO
      throw 13;
    }
  }  // End for

  LVector<ManagedPtr<BitSetIterator>> bit_set_iterators{
      LinearAllocatorAdapterForSTL<ManagedPtr<BitSetIterator>>{allocator}};
  LVector<ManagedPtr<DocIdSetIterator>> non_bit_set_iterators{
      LinearAllocatorAdapterForSTL<ManagedPtr<DocIdSetIterator>>{allocator}};
  for (auto iterator : iterators) {
    if (iterator->get_type_info() == BitSetIterator::TYPE_INFO && iterator->get_cost() > min_cost) {
      // we put all bitset iterators into bit_set_iterators
      // except if they have the minimum get_cost, since we need
      // them to lead the iteration in that case
      bit_set_iterators.emplace_back((BitSetIterator *) iterator);
    } else {
      non_bit_set_iterators.emplace_back(iterator);
    }
  }  // End for

  ManagedPtr<DocIdSetIterator> disi;
  if (iterators.size() == 1) {
    disi = iterators[0];
  } else {
    disi = ConjunctionDISIPcoBase::create(allocator, std::move(non_bit_set_iterators));
  }

  if (bit_set_iterators.size()) {
    disi = allocator->allocate_object<BitSetConjunctionDISI>(disi, std::move(bit_set_iterators));
  }

  if (two_phase_iterators.size()) {
    disi =
        TwoPhaseIterator::as_doc_id_set_iterator(
            allocator->allocate_object<ConjunctionTwoPhaseIterator>(disi, std::move(two_phase_iterators)),
            allocator);
  }

  return disi;
}

//
// ConjunctionUtils
//

ManagedPtr<DocIdSetIterator> ConjunctionUtils::intersect_scorers(
    ManagedPtr<Scorer> *scorers,
    const int32_t num_scorers,
    UseOnlyPtr<LinearAllocator> allocator) {
  if (num_scorers < 2) [[unlikely]] {
    // TODO
    throw 13;
  }

  LVector<ManagedPtr<DocIdSetIterator>> all_iterators{
      LinearAllocatorAdapterForSTL<ManagedPtr<DocIdSetIterator>>{allocator}};
  all_iterators.reserve(num_scorers);
  LVector<ManagedPtr<TwoPhaseIterator>> two_phase_iterators{
      LinearAllocatorAdapterForSTL<ManagedPtr<TwoPhaseIterator>>{allocator}};
  two_phase_iterators.reserve((num_scorers + 1) / 2);

  for (int32_t i = 0; i < num_scorers; ++i) {
    auto scorer = scorers[i];
    add_scorer(scorer, all_iterators, two_phase_iterators);
  }

  return create_conjunction(all_iterators, two_phase_iterators, allocator);
}
