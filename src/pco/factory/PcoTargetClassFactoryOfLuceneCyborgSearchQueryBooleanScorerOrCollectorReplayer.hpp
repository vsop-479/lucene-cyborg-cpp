#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYBOOLEANSCORERORCOLLECTORREPLAYER_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYBOOLEANSCORERORCOLLECTORREPLAYER_HPP_

#include "../../util/LinearAllocator.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../search/query/BooleanScorer.hpp"
#include "../../search/DocIdConsumer.hpp"

namespace lucene::cyborg::pco {

struct PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer {
  static lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::query::BooleanScorerOrCollectorReplayerBase> create(
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::DocIdConsumer>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::search::query::BooleanScorerOrCollector>,
      lucene::cyborg::util::ManagedPtr<lucene::cyborg::util::LinearAllocator>);
};  // PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer

}  // namespace lucene::cyborg::pco

#endif // LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTARGETCLASSFACTORYOFLUCENECYBORGSEARCHQUERYBOOLEANSCORERORCOLLECTORREPLAYER_HPP_
