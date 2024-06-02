#include "DisjunctionDisiApproximation.hpp"

using lucene::cyborg::search::DisiPriorityQueue;
using lucene::cyborg::search::query::DisjunctionDisiApproximation;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;

//
// DisjunctionDisiApproximation
//

DisjunctionDisiApproximation::DisjunctionDisiApproximation(ManagedPtr<DisiPriorityQueue> _sub_iterators)
    : DocIdSetIterator(TypeInfo::get_type_info<DisjunctionDisiApproximation>("DisjunctionDisiApproximation")),
      sub_iterators(_sub_iterators) {
  assert(_sub_iterators);
  int64_t _cost = 0;
  auto internal_array = _sub_iterators->get_internal_array();
  for (int32_t i = 0; i < internal_array.len; ++i) {
    _cost += internal_array.vec[i]->cost;
  }
  cost = _cost;
}

int32_t DisjunctionDisiApproximation::advance(int32_t target) {
  auto top = sub_iterators->top();
  do {
    top->doc = top->approximation->advance(target);
    top = sub_iterators->update_top();
  } while (top->doc < target);

  return top->doc;
}

int32_t DisjunctionDisiApproximation::next_doc() {
  auto top = sub_iterators->top();
  const int32_t doc = top->doc;
  do {
    top->doc = top->approximation->next_doc();
    top = sub_iterators->update_top();
  } while (top->doc == doc);

  return top->doc;
}
