#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHIMPACTSDISIPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHIMPACTSDISIPCO_HPP_

#include "../../search/ImpactsDISIPco.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../search/MaxScoreCache.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../util/LinearAllocator.hpp"

namespace lucene::cyborg::pco {

struct PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco {
  static lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::ImpactsDISIPcoBase> create(
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::MaxScoreCache>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>);
};  // PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco

}  // namespace lucene::cyborg::pco

#endif // LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHIMPACTSDISIPCO_HPP_
