#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHDEFAULTBULKSCORERPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHDEFAULTBULKSCORERPCO_HPP_

#include "../../util/Bits.hpp"
#include "../../search/TwoPhaseIterator.hpp"
#include "../../search/Scorer.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../search/BulkScorer.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../util/LinearAllocator.hpp"
#include "../../search/LeafCollector.hpp"

namespace lucene::cyborg::pco {

struct PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco {
  static lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::BulkScorer> create_for_search(
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdSetIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::TwoPhaseIterator>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::Bits>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::Scorer>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::LeafCollector>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>);
};  // PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco

}  // namespace lucene::cyborg::pco

#endif // LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHDEFAULTBULKSCORERPCO_HPP_
