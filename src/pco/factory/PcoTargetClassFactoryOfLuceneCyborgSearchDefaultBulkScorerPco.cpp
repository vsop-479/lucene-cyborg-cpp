#include "PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco.hpp"
#include "PcoTableModel.hpp"
#include "../../search/ImpactsDISIPco.hpp"
#include "../../search/DefaultBulkScorerPco.hpp"
#include "../../codecs/lucene90/Lucene90PostingsEnum.hpp"
#include "../../search/query/BlockMaxConjunctionScorerIteratorPco.hpp"
#include "../../codecs/lucene90/Lucene90ImpactsEnum.hpp"

using lucene::cyborg::codecs::v90::Lucene90ImpactsEnum;
using lucene::cyborg::codecs::v90::Lucene90PostingsEnum;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::pco::PcoNullType;
using lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco;
using lucene::cyborg::search::BulkScorer;
using lucene::cyborg::search::DefaultBulkScorerPco;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::ImpactsDISIPco;
using lucene::cyborg::search::LeafCollector;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

namespace lucene::cyborg::pco {

//
// Constructor-0, Suffix=[search]
// Target type: [lucene::cyborg::search::DefaultBulkScorerPco]
// Target interface type: [lucene::cyborg::search::BulkScorer]
//

constexpr int CTOR_0_HASH_TABLE_SIZE = 191;
using DefaultBulkScorerPcoCtor0TableKey = PcoTableKey<3>;
using DefaultBulkScorerPcoCtor0FactoryFunction = ManagedPtr<BulkScorer> (*)(ManagedPtr<DocIdSetIterator>,
                                                                            ManagedPtr<TwoPhaseIterator>,
                                                                            ManagedPtr<Bits>,
                                                                            ManagedPtr<Scorer>,
                                                                            ManagedPtr<LeafCollector>,
                                                                            ManagedPtr<LinearAllocator>);

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_0(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::DocIdSetIterator,
                                                         lucene::cyborg::search::TwoPhaseIterator,
                                                         lucene::cyborg::util::Bits>>(((DocIdSetIterator *) param_0),
                                                                                      ((TwoPhaseIterator *) param_1),
                                                                                      ((Bits *) param_2),
                                                                                      param_3,
                                                                                      param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_1(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::DocIdSetIterator,
                                                         lucene::cyborg::search::TwoPhaseIterator,
                                                         lucene::cyborg::pco::PcoNullType>>(((DocIdSetIterator *) param_0),
                                                                                            ((TwoPhaseIterator *) param_1),
                                                                                            ((PcoNullType *) param_2),
                                                                                            param_3,
                                                                                            param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_2(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::DocIdSetIterator,
                                                         lucene::cyborg::pco::PcoNullType,
                                                         lucene::cyborg::util::Bits>>(((DocIdSetIterator *) param_0),
                                                                                      ((PcoNullType *) param_1),
                                                                                      ((Bits *) param_2),
                                                                                      param_3,
                                                                                      param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_3(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::DocIdSetIterator,
                                                         lucene::cyborg::pco::PcoNullType,
                                                         lucene::cyborg::pco::PcoNullType>>(((DocIdSetIterator *) param_0),
                                                                                            ((PcoNullType *) param_1),
                                                                                            ((PcoNullType *) param_2),
                                                                                            param_3,
                                                                                            param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_4(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_5(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_6(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_7(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_8(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_9(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_10(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_11(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_12(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_13(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_14(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_15(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_16(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_17(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_18(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_19(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_20(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_21(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_22(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_23(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_24(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_25(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_26(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_27(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_28(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_29(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_30(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_31(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_32(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_33(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_34(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_35(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_36(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_37(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_38(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_39(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_40(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_41(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_42(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_43(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_44(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_45(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_46(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_47(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_48(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_49(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_50(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_51(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_52(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_53(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_54(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_55(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_56(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_57(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_58(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_59(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_60(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_61(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_62(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_63(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_64(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_65(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_66(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_67(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                                    lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_68(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_69(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_70(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_71(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::search::DocIdSetIterator> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_72(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_73(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_74(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_75(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::NONE>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_76(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_77(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_78(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_79(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_80(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_81(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_82(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_83(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_84(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_85(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_86(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_87(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_88(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                          ((TwoPhaseIterator *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_89(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::search::TwoPhaseIterator,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                                ((TwoPhaseIterator *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_90(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::util::Bits>>(((BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
                                                                              lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                          ((PcoNullType *) param_1),
                                                                          ((Bits *) param_2),
                                                                          param_3,
                                                                          param_4);
}

ManagedPtr<BulkScorer> DefaultBulkScorerPco_ctor_0_factory_func_91(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5
) {
  auto allocator = param_5;

  return allocator
      ->allocate_object<DefaultBulkScorerPco<lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<
          lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>,
                                             lucene::cyborg::pco::PcoNullType,
                                             lucene::cyborg::pco::PcoNullType>>(((BlockMaxConjunctionScorerIteratorPco<
                                                                                    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>> *) param_0),
                                                                                ((PcoNullType *) param_1),
                                                                                ((PcoNullType *) param_2),
                                                                                param_3,
                                                                                param_4);
}

using DefaultBulkScorerPcoCtor0TableCell = PcoTableCell<DefaultBulkScorerPcoCtor0FactoryFunction,
                                                        DefaultBulkScorerPcoCtor0TableKey>;
DefaultBulkScorerPcoCtor0TableCell DefaultBulkScorerPco_CTOR_0_TABLE[CTOR_0_HASH_TABLE_SIZE] = {
    DefaultBulkScorerPcoCtor0TableCell(1,
                                       DefaultBulkScorerPco_ctor_0_factory_func_0,
                                       DefaultBulkScorerPcoCtor0TableKey({11, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(2,
                                       DefaultBulkScorerPco_ctor_0_factory_func_64,
                                       DefaultBulkScorerPcoCtor0TableKey({12, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(3,
                                       DefaultBulkScorerPco_ctor_0_factory_func_12,
                                       DefaultBulkScorerPcoCtor0TableKey({13, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(4,
                                       DefaultBulkScorerPco_ctor_0_factory_func_84,
                                       DefaultBulkScorerPcoCtor0TableKey({14, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(5,
                                       DefaultBulkScorerPco_ctor_0_factory_func_36,
                                       DefaultBulkScorerPcoCtor0TableKey({15, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(6,
                                       DefaultBulkScorerPco_ctor_0_factory_func_24,
                                       DefaultBulkScorerPcoCtor0TableKey({16, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(7,
                                       DefaultBulkScorerPco_ctor_0_factory_func_52,
                                       DefaultBulkScorerPcoCtor0TableKey({17, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(8,
                                       DefaultBulkScorerPco_ctor_0_factory_func_88,
                                       DefaultBulkScorerPcoCtor0TableKey({18, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(9,
                                       DefaultBulkScorerPco_ctor_0_factory_func_44,
                                       DefaultBulkScorerPcoCtor0TableKey({19, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(10,
                                       DefaultBulkScorerPco_ctor_0_factory_func_8,
                                       DefaultBulkScorerPcoCtor0TableKey({20, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(11,
                                       DefaultBulkScorerPco_ctor_0_factory_func_48,
                                       DefaultBulkScorerPcoCtor0TableKey({21, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(12,
                                       DefaultBulkScorerPco_ctor_0_factory_func_20,
                                       DefaultBulkScorerPcoCtor0TableKey({22, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(13,
                                       DefaultBulkScorerPco_ctor_0_factory_func_60,
                                       DefaultBulkScorerPcoCtor0TableKey({23, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(14,
                                       DefaultBulkScorerPco_ctor_0_factory_func_68,
                                       DefaultBulkScorerPcoCtor0TableKey({24, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(15,
                                       DefaultBulkScorerPco_ctor_0_factory_func_28,
                                       DefaultBulkScorerPcoCtor0TableKey({25, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(16,
                                       DefaultBulkScorerPco_ctor_0_factory_func_72,
                                       DefaultBulkScorerPcoCtor0TableKey({26, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(17,
                                       DefaultBulkScorerPco_ctor_0_factory_func_32,
                                       DefaultBulkScorerPcoCtor0TableKey({27, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(18,
                                       DefaultBulkScorerPco_ctor_0_factory_func_40,
                                       DefaultBulkScorerPcoCtor0TableKey({28, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(19,
                                       DefaultBulkScorerPco_ctor_0_factory_func_1,
                                       DefaultBulkScorerPcoCtor0TableKey({11, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(20,
                                       DefaultBulkScorerPco_ctor_0_factory_func_65,
                                       DefaultBulkScorerPcoCtor0TableKey({12, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(21,
                                       DefaultBulkScorerPco_ctor_0_factory_func_13,
                                       DefaultBulkScorerPcoCtor0TableKey({13, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(22,
                                       DefaultBulkScorerPco_ctor_0_factory_func_85,
                                       DefaultBulkScorerPcoCtor0TableKey({14, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(23,
                                       DefaultBulkScorerPco_ctor_0_factory_func_37,
                                       DefaultBulkScorerPcoCtor0TableKey({15, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(24,
                                       DefaultBulkScorerPco_ctor_0_factory_func_25,
                                       DefaultBulkScorerPcoCtor0TableKey({16, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(25,
                                       DefaultBulkScorerPco_ctor_0_factory_func_53,
                                       DefaultBulkScorerPcoCtor0TableKey({17, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(26,
                                       DefaultBulkScorerPco_ctor_0_factory_func_89,
                                       DefaultBulkScorerPcoCtor0TableKey({18, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(27,
                                       DefaultBulkScorerPco_ctor_0_factory_func_45,
                                       DefaultBulkScorerPcoCtor0TableKey({19, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(28,
                                       DefaultBulkScorerPco_ctor_0_factory_func_9,
                                       DefaultBulkScorerPcoCtor0TableKey({20, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(29,
                                       DefaultBulkScorerPco_ctor_0_factory_func_49,
                                       DefaultBulkScorerPcoCtor0TableKey({21, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(30,
                                       DefaultBulkScorerPco_ctor_0_factory_func_21,
                                       DefaultBulkScorerPcoCtor0TableKey({22, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(31,
                                       DefaultBulkScorerPco_ctor_0_factory_func_61,
                                       DefaultBulkScorerPcoCtor0TableKey({23, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(32,
                                       DefaultBulkScorerPco_ctor_0_factory_func_2,
                                       DefaultBulkScorerPcoCtor0TableKey({11, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(33,
                                       DefaultBulkScorerPco_ctor_0_factory_func_66,
                                       DefaultBulkScorerPcoCtor0TableKey({12, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(34,
                                       DefaultBulkScorerPco_ctor_0_factory_func_14,
                                       DefaultBulkScorerPcoCtor0TableKey({13, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(35,
                                       DefaultBulkScorerPco_ctor_0_factory_func_86,
                                       DefaultBulkScorerPcoCtor0TableKey({14, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(36,
                                       DefaultBulkScorerPco_ctor_0_factory_func_38,
                                       DefaultBulkScorerPcoCtor0TableKey({15, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(37,
                                       DefaultBulkScorerPco_ctor_0_factory_func_26,
                                       DefaultBulkScorerPcoCtor0TableKey({16, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(38,
                                       DefaultBulkScorerPco_ctor_0_factory_func_54,
                                       DefaultBulkScorerPcoCtor0TableKey({17, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(39,
                                       DefaultBulkScorerPco_ctor_0_factory_func_90,
                                       DefaultBulkScorerPcoCtor0TableKey({18, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(40,
                                       DefaultBulkScorerPco_ctor_0_factory_func_46,
                                       DefaultBulkScorerPcoCtor0TableKey({19, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(41,
                                       DefaultBulkScorerPco_ctor_0_factory_func_10,
                                       DefaultBulkScorerPcoCtor0TableKey({20, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(42,
                                       DefaultBulkScorerPco_ctor_0_factory_func_50,
                                       DefaultBulkScorerPcoCtor0TableKey({21, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(43,
                                       DefaultBulkScorerPco_ctor_0_factory_func_22,
                                       DefaultBulkScorerPcoCtor0TableKey({22, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(44,
                                       DefaultBulkScorerPco_ctor_0_factory_func_62,
                                       DefaultBulkScorerPcoCtor0TableKey({23, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(45,
                                       DefaultBulkScorerPco_ctor_0_factory_func_69,
                                       DefaultBulkScorerPcoCtor0TableKey({24, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(46,
                                       DefaultBulkScorerPco_ctor_0_factory_func_70,
                                       DefaultBulkScorerPcoCtor0TableKey({24, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(47,
                                       DefaultBulkScorerPco_ctor_0_factory_func_29,
                                       DefaultBulkScorerPcoCtor0TableKey({25, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(48,
                                       DefaultBulkScorerPco_ctor_0_factory_func_30,
                                       DefaultBulkScorerPcoCtor0TableKey({25, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(49,
                                       DefaultBulkScorerPco_ctor_0_factory_func_73,
                                       DefaultBulkScorerPcoCtor0TableKey({26, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(50,
                                       DefaultBulkScorerPco_ctor_0_factory_func_3,
                                       DefaultBulkScorerPcoCtor0TableKey({11, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(51,
                                       DefaultBulkScorerPco_ctor_0_factory_func_67,
                                       DefaultBulkScorerPcoCtor0TableKey({12, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(52,
                                       DefaultBulkScorerPco_ctor_0_factory_func_15,
                                       DefaultBulkScorerPcoCtor0TableKey({13, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(53,
                                       DefaultBulkScorerPco_ctor_0_factory_func_87,
                                       DefaultBulkScorerPcoCtor0TableKey({14, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(54,
                                       DefaultBulkScorerPco_ctor_0_factory_func_39,
                                       DefaultBulkScorerPcoCtor0TableKey({15, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(55,
                                       DefaultBulkScorerPco_ctor_0_factory_func_27,
                                       DefaultBulkScorerPcoCtor0TableKey({16, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(56,
                                       DefaultBulkScorerPco_ctor_0_factory_func_55,
                                       DefaultBulkScorerPcoCtor0TableKey({17, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(57,
                                       DefaultBulkScorerPco_ctor_0_factory_func_91,
                                       DefaultBulkScorerPcoCtor0TableKey({18, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(58,
                                       DefaultBulkScorerPco_ctor_0_factory_func_47,
                                       DefaultBulkScorerPcoCtor0TableKey({19, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(59,
                                       DefaultBulkScorerPco_ctor_0_factory_func_11,
                                       DefaultBulkScorerPcoCtor0TableKey({20, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(60,
                                       DefaultBulkScorerPco_ctor_0_factory_func_51,
                                       DefaultBulkScorerPcoCtor0TableKey({21, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(61,
                                       DefaultBulkScorerPco_ctor_0_factory_func_23,
                                       DefaultBulkScorerPcoCtor0TableKey({22, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(62,
                                       DefaultBulkScorerPco_ctor_0_factory_func_63,
                                       DefaultBulkScorerPcoCtor0TableKey({23, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(63,
                                       DefaultBulkScorerPco_ctor_0_factory_func_71,
                                       DefaultBulkScorerPcoCtor0TableKey({24, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(64,
                                       DefaultBulkScorerPco_ctor_0_factory_func_31,
                                       DefaultBulkScorerPcoCtor0TableKey({25, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(65,
                                       DefaultBulkScorerPco_ctor_0_factory_func_74,
                                       DefaultBulkScorerPcoCtor0TableKey({26, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(66,
                                       DefaultBulkScorerPco_ctor_0_factory_func_75,
                                       DefaultBulkScorerPcoCtor0TableKey({26, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(67,
                                       DefaultBulkScorerPco_ctor_0_factory_func_33,
                                       DefaultBulkScorerPcoCtor0TableKey({27, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(68,
                                       DefaultBulkScorerPco_ctor_0_factory_func_34,
                                       DefaultBulkScorerPcoCtor0TableKey({27, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(69,
                                       DefaultBulkScorerPco_ctor_0_factory_func_35,
                                       DefaultBulkScorerPcoCtor0TableKey({27, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(70,
                                       DefaultBulkScorerPco_ctor_0_factory_func_41,
                                       DefaultBulkScorerPcoCtor0TableKey({28, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(71,
                                       DefaultBulkScorerPco_ctor_0_factory_func_42,
                                       DefaultBulkScorerPcoCtor0TableKey({28, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(72,
                                       DefaultBulkScorerPco_ctor_0_factory_func_43,
                                       DefaultBulkScorerPcoCtor0TableKey({28, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(73,
                                       DefaultBulkScorerPco_ctor_0_factory_func_76,
                                       DefaultBulkScorerPcoCtor0TableKey({29, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(74,
                                       DefaultBulkScorerPco_ctor_0_factory_func_77,
                                       DefaultBulkScorerPcoCtor0TableKey({29, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(75,
                                       DefaultBulkScorerPco_ctor_0_factory_func_78,
                                       DefaultBulkScorerPcoCtor0TableKey({29, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(76,
                                       DefaultBulkScorerPco_ctor_0_factory_func_79,
                                       DefaultBulkScorerPcoCtor0TableKey({29, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(77,
                                       DefaultBulkScorerPco_ctor_0_factory_func_4,
                                       DefaultBulkScorerPcoCtor0TableKey({30, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(78,
                                       DefaultBulkScorerPco_ctor_0_factory_func_5,
                                       DefaultBulkScorerPcoCtor0TableKey({30, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(79,
                                       DefaultBulkScorerPco_ctor_0_factory_func_6,
                                       DefaultBulkScorerPcoCtor0TableKey({30, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(80,
                                       DefaultBulkScorerPco_ctor_0_factory_func_7,
                                       DefaultBulkScorerPcoCtor0TableKey({30, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(81,
                                       DefaultBulkScorerPco_ctor_0_factory_func_16,
                                       DefaultBulkScorerPcoCtor0TableKey({31, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(82,
                                       DefaultBulkScorerPco_ctor_0_factory_func_17,
                                       DefaultBulkScorerPcoCtor0TableKey({31, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(83,
                                       DefaultBulkScorerPco_ctor_0_factory_func_18,
                                       DefaultBulkScorerPcoCtor0TableKey({31, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(84,
                                       DefaultBulkScorerPco_ctor_0_factory_func_19,
                                       DefaultBulkScorerPcoCtor0TableKey({31, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(85,
                                       DefaultBulkScorerPco_ctor_0_factory_func_80,
                                       DefaultBulkScorerPcoCtor0TableKey({32, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(86,
                                       DefaultBulkScorerPco_ctor_0_factory_func_81,
                                       DefaultBulkScorerPcoCtor0TableKey({32, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(87,
                                       DefaultBulkScorerPco_ctor_0_factory_func_82,
                                       DefaultBulkScorerPcoCtor0TableKey({32, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(88,
                                       DefaultBulkScorerPco_ctor_0_factory_func_83,
                                       DefaultBulkScorerPcoCtor0TableKey({32, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(89,
                                       DefaultBulkScorerPco_ctor_0_factory_func_56,
                                       DefaultBulkScorerPcoCtor0TableKey({33, 7, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(90,
                                       DefaultBulkScorerPco_ctor_0_factory_func_57,
                                       DefaultBulkScorerPcoCtor0TableKey({33, 7, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(91,
                                       DefaultBulkScorerPco_ctor_0_factory_func_58,
                                       DefaultBulkScorerPcoCtor0TableKey({33, 8, 5}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(0,
                                       DefaultBulkScorerPco_ctor_0_factory_func_59,
                                       DefaultBulkScorerPcoCtor0TableKey({33, 8, 8}, 129)),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(),
    DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell(), DefaultBulkScorerPcoCtor0TableCell()
};

ManagedPtr<BulkScorer> PcoTargetClassFactoryOfLuceneCyborgSearchDefaultBulkScorerPco::create_for_search(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<TwoPhaseIterator> param_1,
    ManagedPtr<Bits> param_2,
    ManagedPtr<Scorer> param_3,
    ManagedPtr<LeafCollector> param_4,
    ManagedPtr<LinearAllocator> param_5) {
  auto allocator = param_5;

  const auto key = DefaultBulkScorerPcoCtor0TableKey({
                                                         param_0 != nullptr ? (((PcoTypeIndexSupport *) param_0)
                                                             ->get_pco_type_index()) : (8),
                                                         param_1 != nullptr ? (((PcoTypeIndexSupport *) param_1)
                                                             ->get_pco_type_index()) : (8),
                                                         param_2 != nullptr ? (((PcoTypeIndexSupport *) param_2)
                                                             ->get_pco_type_index()) : (8)
                                                     },
                                                     129);

  auto factory = PcoTableLookUp::look_up<DefaultBulkScorerPcoCtor0TableKey,
                                         DefaultBulkScorerPcoCtor0TableCell,
                                         CTOR_0_HASH_TABLE_SIZE>(key, &DefaultBulkScorerPco_CTOR_0_TABLE[0]);

  return factory(param_0, param_1, param_2, param_3, param_4, allocator);
}

}  // namespace lucene::cyborg::pco
