#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSSOURCE_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSSOURCE_HPP_

#include <cstdint>
#include "../util/PointerTypes.hpp"
#include "Impacts.hpp"

namespace lucene::cyborg::index {

class ImpactsSource {
 public:
  virtual void advance_shallow(int32_t target) = 0;

  virtual util::ManagedPtr<Impacts> get_impacts() = 0;
};  // ImpactsSource



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSSOURCE_HPP_
