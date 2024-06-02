#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHBLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHBLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_

#include "../../util/LinearAllocator.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../search/query/BlockMaxConjunctionScorer.hpp"

namespace lucene::cyborg::pco {

struct PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco {
  static lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator> create_for_search(
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::BlockMaxConjunctionScorer>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>);
};  // PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco

}  // namespace lucene::cyborg::pco

#endif // LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHBLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_
