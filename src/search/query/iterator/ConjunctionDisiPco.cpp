#include <algorithm>
#include "ConjunctionDisiPco.hpp"
#include "../../../pco/factory/PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco.hpp"

using lucene::cyborg::pco::PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::query::ConjunctionDISIPcoBase;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::UseOnlyPtr;

const TypeInfoPtr ConjunctionDISIPcoBase::TYPE_INFO =
    TypeInfo::get_type_info<ConjunctionDISIPcoBase>("ConjunctionDISIPcoBase");

ManagedPtr<DocIdSetIterator> ConjunctionDISIPcoBase::create(UseOnlyPtr<LinearAllocator> allocator,
                                                            LVector<ManagedPtr<DocIdSetIterator>> &&iterators) {
  std::sort(iterators.begin(), iterators.end(), [](auto iter0, auto iter1) {
    return iter0->get_cost() < iter1->get_cost();
  });

  auto lead1 = iterators[0];
  auto lead2 = iterators[1];

  return PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco::create(lead1, lead2, &iterators, allocator);
}
