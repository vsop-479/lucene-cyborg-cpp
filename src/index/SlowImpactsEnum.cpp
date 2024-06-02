#include <limits>
#include "SlowImpactsEnum.hpp"

using lucene::cyborg::index::Impact;
using lucene::cyborg::index::Impacts;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::SlowImpactsDummyImpacts;
using lucene::cyborg::index::SlowImpactsEnum;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::TypedVector;

//
// SlowImpactsEnum
//

const TypeInfo SlowImpactsEnumTypeInfo{"SlowImpactsEnum"};

SlowImpactsEnum::SlowImpactsEnum(ManagedPtr<PostingsEnum> _delegate)
    : ImpactsEnum(&SlowImpactsEnumTypeInfo),
      delegate(_delegate) {
}

int32_t SlowImpactsEnum::get_doc_id() {
  return delegate->get_doc_id();
}

int32_t SlowImpactsEnum::get_freq() {
  return delegate->get_freq();
}

int32_t SlowImpactsEnum::next_doc() {
  return delegate->next_doc();
}

int64_t SlowImpactsEnum::get_cost() {
  return delegate->get_cost();
}

int32_t SlowImpactsEnum::advance(int32_t target) {
  return delegate->advance(target);
}

ManagedPtr<SlowImpactsDummyImpacts> SlowImpactsEnum::get_impacts() {
  static SlowImpactsDummyImpacts DUMMY_IMPACTS{};
  return &DUMMY_IMPACTS;
}

//
// SlowImpactsDummyImpacts
//

static const int32_t SlowImpactsDummyImpactsNumLevels = 1;

SlowImpactsDummyImpacts::SlowImpactsDummyImpacts()
    : Impacts(&SlowImpactsDummyImpactsNumLevels) {
}

int32_t SlowImpactsDummyImpacts::get_doc_id_up_to(int32_t level) {
  return DocIdSetIterator::NO_MORE_DOCS;
}

TypedVector<Impact> SlowImpactsDummyImpacts::get_impacts(int32_t level) {
  static Impact DUMMY_IMPACT{std::numeric_limits<int32_t>::max(), 1};
  return TypedVector<Impact>{&DUMMY_IMPACT, 1L};
}
