#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYCONJUNCTIONDISIPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYCONJUNCTIONDISIPCO_HPP_

#include "../../util/LinearAllocator.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../search/DocIdSetIterator.hpp"

namespace lucene::cyborg::pco {

struct PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco {
  static lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator> create(
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LVector<util::ManagedPtr<search::DocIdSetIterator>>>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>);
};  // PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco

}  // namespace lucene::cyborg::pco

#endif // LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYCONJUNCTIONDISIPCO_HPP_
