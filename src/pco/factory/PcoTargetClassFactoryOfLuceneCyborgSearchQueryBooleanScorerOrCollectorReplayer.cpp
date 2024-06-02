#include "PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer.hpp"
#include "PcoTableModel.hpp"
#include "../..//search/MatchCountCollectorManager.hpp"

using lucene::cyborg::search::DefaultCountDocIdStream;
using lucene::cyborg::search::DocIdConsumer;
using lucene::cyborg::search::MatchCountLeafCollector;
using lucene::cyborg::search::query::BooleanScorerOrCollector;
using lucene::cyborg::search::query::BooleanScorerOrCollectorReplayer;
using lucene::cyborg::search::query::BooleanScorerOrCollectorReplayerBase;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

namespace lucene::cyborg::pco {

//
// Constructor-0, Suffix=[]
// Target type: [lucene::cyborg::search::query::BooleanScorerOrCollectorReplayer]
// Target interface type: [lucene::cyborg::search::query::BooleanScorerOrCollectorReplayerBase]
//

constexpr int CTOR_0_HASH_TABLE_SIZE = 7;
using BooleanScorerOrCollectorReplayerCtor0TableKey = PcoTableKey<1>;
using BooleanScorerOrCollectorReplayerCtor0FactoryFunction = ManagedPtr<BooleanScorerOrCollectorReplayerBase> (*)(
    ManagedPtr<DocIdConsumer>,
    ManagedPtr<BooleanScorerOrCollector>,
    ManagedPtr<LinearAllocator>);

ManagedPtr<BooleanScorerOrCollectorReplayerBase> BooleanScorerOrCollectorReplayer_ctor_0_factory_func_0(
    ManagedPtr<DocIdConsumer> param_0, ManagedPtr<BooleanScorerOrCollector> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BooleanScorerOrCollectorReplayer<lucene::cyborg::search::DocIdConsumer>>(((DocIdConsumer *) param_0),
                                                                                                 param_1);
}

ManagedPtr<BooleanScorerOrCollectorReplayerBase> BooleanScorerOrCollectorReplayer_ctor_0_factory_func_1(
    ManagedPtr<DocIdConsumer> param_0, ManagedPtr<BooleanScorerOrCollector> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BooleanScorerOrCollectorReplayer<lucene::cyborg::search::MatchCountLeafCollector>>(((MatchCountLeafCollector *) param_0),
                                                                                                           param_1);
}

ManagedPtr<BooleanScorerOrCollectorReplayerBase> BooleanScorerOrCollectorReplayer_ctor_0_factory_func_2(
    ManagedPtr<DocIdConsumer> param_0, ManagedPtr<BooleanScorerOrCollector> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BooleanScorerOrCollectorReplayer<lucene::cyborg::search::DefaultCountDocIdStream>>(((DefaultCountDocIdStream *) param_0),
                                                                                                           param_1);
}

using BooleanScorerOrCollectorReplayerCtor0TableCell = PcoTableCell<BooleanScorerOrCollectorReplayerCtor0FactoryFunction,
                                                                    BooleanScorerOrCollectorReplayerCtor0TableKey>;
BooleanScorerOrCollectorReplayerCtor0TableCell BooleanScorerOrCollectorReplayer_CTOR_0_TABLE[CTOR_0_HASH_TABLE_SIZE] = {
    BooleanScorerOrCollectorReplayerCtor0TableCell(3,
                                                   BooleanScorerOrCollectorReplayer_ctor_0_factory_func_0,
                                                   BooleanScorerOrCollectorReplayerCtor0TableKey({10}, 1)),
    BooleanScorerOrCollectorReplayerCtor0TableCell(), BooleanScorerOrCollectorReplayerCtor0TableCell(),
    BooleanScorerOrCollectorReplayerCtor0TableCell(6,
                                                   BooleanScorerOrCollectorReplayer_ctor_0_factory_func_2,
                                                   BooleanScorerOrCollectorReplayerCtor0TableKey({6}, 1)),
    BooleanScorerOrCollectorReplayerCtor0TableCell(), BooleanScorerOrCollectorReplayerCtor0TableCell(),
    BooleanScorerOrCollectorReplayerCtor0TableCell(0,
                                                   BooleanScorerOrCollectorReplayer_ctor_0_factory_func_1,
                                                   BooleanScorerOrCollectorReplayerCtor0TableKey({9}, 1))
};

ManagedPtr<BooleanScorerOrCollectorReplayerBase> PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer::create(
    ManagedPtr<DocIdConsumer> param_0,
    ManagedPtr<BooleanScorerOrCollector> param_1,
    ManagedPtr<LinearAllocator> param_2) {
  auto allocator = param_2;

  const auto key = BooleanScorerOrCollectorReplayerCtor0TableKey({
                                                                     param_0 != nullptr
                                                                     ? (((PcoTypeIndexSupport *) param_0)
                                                                         ->get_pco_type_index()) : (8)
                                                                 },
                                                                 1);

  auto factory = PcoTableLookUp::look_up<BooleanScorerOrCollectorReplayerCtor0TableKey,
                                         BooleanScorerOrCollectorReplayerCtor0TableCell,
                                         CTOR_0_HASH_TABLE_SIZE>(key,
                                                                 &BooleanScorerOrCollectorReplayer_CTOR_0_TABLE[0]);

  return factory(param_0, param_1, allocator);
}

}  // namespace lucene::cyborg::pco
