#include "PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco.hpp"
#include "PcoTableModel.hpp"
#include "../../search/query/BlockMaxConjunctionScorerIteratorPco.hpp"
#include "../../codecs/lucene90/Lucene90ImpactsEnum.hpp"

using lucene::cyborg::codecs::v90::Lucene90ImpactsEnum;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::search::BlockMaxConjunctionScorer;
using lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

namespace lucene::cyborg::pco {

//
// Constructor-0, Suffix=[search]
// Target type: [lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco]
// Target interface type: [lucene::cyborg::search::DocIdSetIterator]
//

constexpr int CTOR_0_HASH_TABLE_SIZE = 13;
using BlockMaxConjunctionScorerIteratorPcoCtor0TableKey = PcoTableKey<1>;
using BlockMaxConjunctionScorerIteratorPcoCtor0FactoryFunction = ManagedPtr<DocIdSetIterator> (*)(ManagedPtr<DocIdSetIterator>,
                                                                                                  ManagedPtr<BlockMaxConjunctionScorer>,
                                                                                                  ManagedPtr<LinearAllocator>);

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_0(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>>(((DocIdSetIterator *) param_0),
                                                                                                        param_1);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_1(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          param_1);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_2(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          param_1);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_3(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          param_1);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_4(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          param_1);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_5(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          param_1);
}

using BlockMaxConjunctionScorerIteratorPcoCtor0TableCell = PcoTableCell<
    BlockMaxConjunctionScorerIteratorPcoCtor0FactoryFunction,
    BlockMaxConjunctionScorerIteratorPcoCtor0TableKey>;
BlockMaxConjunctionScorerIteratorPcoCtor0TableCell
    BlockMaxConjunctionScorerIteratorPco_CTOR_0_TABLE[CTOR_0_HASH_TABLE_SIZE] = {
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(1,
                                                       BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_0,
                                                       BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({11}, 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(3,
                                                       BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_2,
                                                       BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({25}, 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(), BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(4,
                                                                                                             BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_3,
                                                                                                             BlockMaxConjunctionScorerIteratorPcoCtor0TableKey(
                                                                                                                 {27},
                                                                                                                 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(5,
                                                       BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_4,
                                                       BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({15}, 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(6,
                                                       BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_1,
                                                       BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({16}, 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(0,
                                                       BlockMaxConjunctionScorerIteratorPco_ctor_0_factory_func_5,
                                                       BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({28}, 10)),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(), BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(), BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(),
    BlockMaxConjunctionScorerIteratorPcoCtor0TableCell(), BlockMaxConjunctionScorerIteratorPcoCtor0TableCell()
};

ManagedPtr<DocIdSetIterator> PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco::create_for_search(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<BlockMaxConjunctionScorer> param_1,
    ManagedPtr<LinearAllocator> param_2) {
  auto allocator = param_2;

  const auto key = BlockMaxConjunctionScorerIteratorPcoCtor0TableKey({
                                                                         param_0 != nullptr
                                                                         ? (((PcoTypeIndexSupport *) param_0)
                                                                             ->get_pco_type_index()) : (8)
                                                                     },
                                                                     10);

  auto factory = PcoTableLookUp::look_up<BlockMaxConjunctionScorerIteratorPcoCtor0TableKey,
                                         BlockMaxConjunctionScorerIteratorPcoCtor0TableCell,
                                         CTOR_0_HASH_TABLE_SIZE>(key,
                                                                 &BlockMaxConjunctionScorerIteratorPco_CTOR_0_TABLE[0]);

  return factory(param_0, param_1, allocator);
}

}  // namespace lucene::cyborg::pco
