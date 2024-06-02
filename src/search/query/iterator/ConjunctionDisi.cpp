#include <algorithm>
#include "ConjunctionDisi.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::query::ConjunctionDISI;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;

//
// ConjunctionDISI
//

const TypeInfoPtr ConjunctionDISI::TYPE_INFO =
    TypeInfo::get_type_info<ConjunctionDISI>("ConjunctionDISI");

ConjunctionDISI::ConjunctionDISI(LVector<ManagedPtr<DocIdSetIterator>> &&_iterators)
    : DocIdSetIterator(TYPE_INFO),
      lead1(),
      lead2(),
      others() {
  assert(_iterators.size() >= 2);

  std::sort(_iterators.begin(), _iterators.end(), [](auto iter0, auto iter1) {
    return iter0->get_cost() < iter1->get_cost();
  });

  lead1 = _iterators[0];
  lead2 = _iterators[1];
  others = std::move(_iterators);
}

int64_t ConjunctionDISI::get_cost() {
  return lead1->get_cost();
}

int32_t ConjunctionDISI::advance(int32_t target) {
  assert(assert_iters_on_same_doc());
  return do_next(lead1->advance(target));
}

int32_t ConjunctionDISI::get_doc_id() {
  return lead1->get_doc_id();
}

int32_t ConjunctionDISI::next_doc() {
  assert(assert_iters_on_same_doc());
  return do_next(lead1->next_doc());
}

int32_t ConjunctionDISI::do_next(int32_t doc) {
  while (true) {
    ADVANCE_HEAD:
    assert(doc == lead1->get_doc_id());

    const int32_t next2 = lead2->advance(doc);
    if (next2 != doc) {
      doc = lead1->advance(next2);
      if (next2 != doc) {
        goto ADVANCE_HEAD;
      }
    }

    for (int32_t i = 2; i < others.size(); ++i) {
      auto &other = others[i];
      if (other->get_doc_id() < doc) {
        const int32_t next = other->advance(doc);

        if (next > doc) {
          doc = lead1->advance(next);
          goto ADVANCE_HEAD;
        }
      }
    }

    return doc;
  }  // End while
}

#ifndef NDEBUG
bool ConjunctionDISI::assert_iters_on_same_doc() {
  auto cur_doc = lead1->get_doc_id();
  bool iterators_on_the_same_doc = (lead2->get_doc_id() == cur_doc);
  for (int32_t i = 0; (i < others.size() && iterators_on_the_same_doc); ++i) {
    iterators_on_the_same_doc = iterators_on_the_same_doc && (others[i]->get_doc_id() == cur_doc);
  }
  return iterators_on_the_same_doc;
}
#endif
