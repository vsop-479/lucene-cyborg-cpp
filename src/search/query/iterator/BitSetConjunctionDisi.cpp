#include <algorithm>
#include <limits>
#include "BitSetConjunctionDisi.hpp"

using lucene::cyborg::search::query::BitSetConjunctionDISI;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::LVector;

//
// BitSetConjunctionDISI
//

const TypeInfoPtr BitSetConjunctionDISI::TYPE_INFO =
    TypeInfo::get_type_info<BitSetConjunctionDISI>("BitSetConjunctionDISI");

BitSetConjunctionDISI::BitSetConjunctionDISI(ManagedPtr<DocIdSetIterator> _lead,
                                             LVector<ManagedPtr<BitSetIterator>> &&_bit_set_iterators)
    : DocIdSetIterator(BitSetConjunctionDISI::TYPE_INFO),
      lead(_lead),
      bit_set_iterators(std::move(_bit_set_iterators)),
      bit_sets(),
      min_length() {
  assert(!bit_set_iterators.empty());

  std::sort(bit_set_iterators.begin(), bit_set_iterators.end(), [](auto bsi_0, auto bsi_1) {
    return bsi_0->get_cost() < bsi_1->get_cost();
  });

  int32_t min_length = std::numeric_limits<int32_t>::max();
  bit_sets.reserve(bit_set_iterators.size());
  for (auto bit_set_iterator : bit_set_iterators) {
    bit_sets.emplace_back(bit_set_iterator->get_bit_set());
    if (auto length = bit_sets.back()->get_length(); length < min_length) {
      min_length = length;
    }
  }
  this->min_length = min_length;
}

int64_t BitSetConjunctionDISI::get_cost() {
  return lead->get_cost();
}

int32_t BitSetConjunctionDISI::advance(int32_t target) {
  assert(assert_iterators_on_same_doc());
  return do_next(lead->advance(target));
}

int32_t BitSetConjunctionDISI::get_doc_id() {
  return lead->get_doc_id();
}

int32_t BitSetConjunctionDISI::next_doc() {
  assert(assert_iterators_on_same_doc());
  return do_next(lead->next_doc());
}

int32_t BitSetConjunctionDISI::do_next(int32_t doc) {
  while (true) {
    OUTER_LOOP_CONTINUE:

    if (doc < min_length) {
      for (auto bit_set : bit_sets) {
        if (!bit_set->get(doc)) {
          doc = lead->next_doc();
          goto OUTER_LOOP_CONTINUE;
        }
      }
      for (auto iterator : bit_set_iterators) {
        iterator->set_doc_id(doc);
      }
      return doc;
    } else {
      if (doc != NO_MORE_DOCS) {
        lead->advance(NO_MORE_DOCS);
      }
      return NO_MORE_DOCS;
    }  // End if
  }  // End for
}

#ifndef NDEBUG
bool BitSetConjunctionDISI::assert_iterators_on_same_doc() {
  int32_t cur_doc = lead->get_doc_id();
  bool iterators_on_the_same_doc = true;
  for (int32_t i = 0; (iterators_on_the_same_doc && i < bit_set_iterators.size()); ++i) {
    iterators_on_the_same_doc = iterators_on_the_same_doc && (bit_set_iterators[i]->get_doc_id() == cur_doc);
  }
  return iterators_on_the_same_doc;
}
#endif