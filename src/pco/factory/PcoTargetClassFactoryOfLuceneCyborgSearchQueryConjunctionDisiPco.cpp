#include "PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco.hpp"
#include "PcoTableModel.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../search/query/iterator/ConjunctionDisiPco.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../codecs/lucene90/Lucene90PostingsEnum.hpp"
#include "../../index/IndexOptions.hpp"

using lucene::cyborg::codecs::v90::Lucene90PostingsEnum;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::query::ConjunctionDisiPco;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

namespace lucene::cyborg::pco {

//
// Constructor-0, Suffix=[]
// Target type: [lucene::cyborg::search::query::ConjunctionDisiPco]
// Target interface type: [lucene::cyborg::search::DocIdSetIterator]
//

constexpr int CTOR_0_HASH_TABLE_SIZE = 251;
using ConjunctionDisiPcoCtor0TableKey = PcoTableKey<2>;
using ConjunctionDisiPcoCtor0FactoryFunction = ManagedPtr<DocIdSetIterator> (*)(ManagedPtr<DocIdSetIterator>,
                                                                                ManagedPtr<DocIdSetIterator>,
                                                                                ManagedPtr<LVector<util::ManagedPtr<
                                                                                    search::DocIdSetIterator>>>,
                                                                                ManagedPtr<LinearAllocator>);

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_0(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::search::DocIdSetIterator>>(((DocIdSetIterator *) param_0),
                                                                                                  ((DocIdSetIterator *) param_1),
                                                                                                  param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_1(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_2(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_3(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_4(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_5(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_6(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_7(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_8(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_9(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_10(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator->allocate_object<ConjunctionDisiPco<lucene::cyborg::search::DocIdSetIterator,
                                                       lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
      ((DocIdSetIterator *) param_0),
      ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_11(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_12(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_13(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_14(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_15(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_16(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_17(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_18(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_19(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_20(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_21(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_22(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_23(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_24(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_25(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_26(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_27(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_28(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_29(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_30(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_31(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_32(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_33(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_34(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_35(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_36(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_37(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_38(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_39(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_40(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_41(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_42(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_43(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_44(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_45(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_46(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_47(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_48(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_49(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_50(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_51(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_52(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_53(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_54(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_55(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_56(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_57(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_58(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_59(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_60(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_61(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_62(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_63(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_64(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_65(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_66(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_67(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_68(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_69(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_70(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_71(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_72(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_73(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_74(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_75(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_76(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_77(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_78(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_79(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_80(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_81(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_82(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_83(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_84(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_85(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_86(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_87(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_88(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_89(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_90(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_91(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_92(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_93(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_94(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_95(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_96(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_97(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_98(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_99(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_100(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_101(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_102(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_103(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_104(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_105(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_106(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_107(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_108(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_109(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_110(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::search::DocIdSetIterator>>(((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
                                                                                      ((DocIdSetIterator *) param_1),
                                                                                      param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_111(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_112(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_113(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_114(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_115(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_116(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_117(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_118(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_119(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_1),
          param_2);
}

ManagedPtr<DocIdSetIterator> ConjunctionDisiPco_ctor_0_factory_func_120(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3
) {
  auto allocator = param_3;

  return allocator
      ->allocate_object<ConjunctionDisiPco<lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>,
                                           lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          ((Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_1),
          param_2);
}

using ConjunctionDisiPcoCtor0TableCell = PcoTableCell<ConjunctionDisiPcoCtor0FactoryFunction,
                                                      ConjunctionDisiPcoCtor0TableKey>;
ConjunctionDisiPcoCtor0TableCell ConjunctionDisiPco_CTOR_0_TABLE[CTOR_0_HASH_TABLE_SIZE] = {
    ConjunctionDisiPcoCtor0TableCell(2,
                                     ConjunctionDisiPco_ctor_0_factory_func_0,
                                     ConjunctionDisiPcoCtor0TableKey({11, 11}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(3,
                                                                         ConjunctionDisiPco_ctor_0_factory_func_33,
                                                                         ConjunctionDisiPcoCtor0TableKey({13, 11},
                                                                                                         193)),
    ConjunctionDisiPcoCtor0TableCell(9,
                                     ConjunctionDisiPco_ctor_0_factory_func_88,
                                     ConjunctionDisiPcoCtor0TableKey({13, 11}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(10,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_22,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {20,
                                                                                                                  11},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(11,
                                     ConjunctionDisiPco_ctor_0_factory_func_77,
                                     ConjunctionDisiPcoCtor0TableKey({20, 11}, 193)), ConjunctionDisiPcoCtor0TableCell(
        12,
        ConjunctionDisiPco_ctor_0_factory_func_55,
        ConjunctionDisiPcoCtor0TableKey({22, 11}, 193)), ConjunctionDisiPcoCtor0TableCell(19,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_110,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 11},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(20,
                                     ConjunctionDisiPco_ctor_0_factory_func_11,
                                     ConjunctionDisiPcoCtor0TableKey({30, 11}, 193)), ConjunctionDisiPcoCtor0TableCell(
        21,
        ConjunctionDisiPco_ctor_0_factory_func_66,
        ConjunctionDisiPcoCtor0TableKey({30, 11}, 193)), ConjunctionDisiPcoCtor0TableCell(22,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_44,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 11},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(28,
                                     ConjunctionDisiPco_ctor_0_factory_func_99,
                                     ConjunctionDisiPcoCtor0TableKey({31, 11}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(29,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_2,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {11,
                                                                                                                  20},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(30,
                                     ConjunctionDisiPco_ctor_0_factory_func_7,
                                     ConjunctionDisiPcoCtor0TableKey({11, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        31,
        ConjunctionDisiPco_ctor_0_factory_func_35,
        ConjunctionDisiPcoCtor0TableKey({13, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(32,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_40,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 20},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(33,
                                     ConjunctionDisiPco_ctor_0_factory_func_90,
                                     ConjunctionDisiPcoCtor0TableKey({13, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        37,
        ConjunctionDisiPco_ctor_0_factory_func_95,
        ConjunctionDisiPcoCtor0TableKey({13, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(38,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_24,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {20,
                                                                                                                  20},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(39,
                                     ConjunctionDisiPco_ctor_0_factory_func_29,
                                     ConjunctionDisiPcoCtor0TableKey({20, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        40,
        ConjunctionDisiPco_ctor_0_factory_func_79,
        ConjunctionDisiPcoCtor0TableKey({20, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(41,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_84,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 20},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(42,
                                     ConjunctionDisiPco_ctor_0_factory_func_57,
                                     ConjunctionDisiPcoCtor0TableKey({22, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        43,
        ConjunctionDisiPco_ctor_0_factory_func_62,
        ConjunctionDisiPcoCtor0TableKey({22, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(44,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_112,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 20},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(47,
                                     ConjunctionDisiPco_ctor_0_factory_func_117,
                                     ConjunctionDisiPcoCtor0TableKey({22, 20}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(48,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_13,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {30,
                                                                                                                  20},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(49,
                                     ConjunctionDisiPco_ctor_0_factory_func_18,
                                     ConjunctionDisiPcoCtor0TableKey({30, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        50,
        ConjunctionDisiPco_ctor_0_factory_func_68,
        ConjunctionDisiPcoCtor0TableKey({30, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(51,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_73,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 20},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(52,
                                     ConjunctionDisiPco_ctor_0_factory_func_46,
                                     ConjunctionDisiPcoCtor0TableKey({31, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(
        53,
        ConjunctionDisiPco_ctor_0_factory_func_51,
        ConjunctionDisiPcoCtor0TableKey({31, 20}, 193)), ConjunctionDisiPcoCtor0TableCell(54,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_101,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 20},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(62,
                                     ConjunctionDisiPco_ctor_0_factory_func_106,
                                     ConjunctionDisiPcoCtor0TableKey({31, 20}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(63,
                                                                         ConjunctionDisiPco_ctor_0_factory_func_3,
                                                                         ConjunctionDisiPcoCtor0TableKey({11, 13},
                                                                                                         193)),
    ConjunctionDisiPcoCtor0TableCell(64,
                                     ConjunctionDisiPco_ctor_0_factory_func_8,
                                     ConjunctionDisiPcoCtor0TableKey({11, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        65,
        ConjunctionDisiPco_ctor_0_factory_func_36,
        ConjunctionDisiPcoCtor0TableKey({13, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(66,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_41,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 13},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(67,
                                     ConjunctionDisiPco_ctor_0_factory_func_91,
                                     ConjunctionDisiPcoCtor0TableKey({13, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        71,
        ConjunctionDisiPco_ctor_0_factory_func_96,
        ConjunctionDisiPcoCtor0TableKey({13, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(72,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_25,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {20,
                                                                                                                  13},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(73,
                                     ConjunctionDisiPco_ctor_0_factory_func_30,
                                     ConjunctionDisiPcoCtor0TableKey({20, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        74,
        ConjunctionDisiPco_ctor_0_factory_func_80,
        ConjunctionDisiPcoCtor0TableKey({20, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(75,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_85,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 13},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(76,
                                     ConjunctionDisiPco_ctor_0_factory_func_58,
                                     ConjunctionDisiPcoCtor0TableKey({22, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        77,
        ConjunctionDisiPco_ctor_0_factory_func_63,
        ConjunctionDisiPcoCtor0TableKey({22, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(78,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_113,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 13},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(81,
                                     ConjunctionDisiPco_ctor_0_factory_func_118,
                                     ConjunctionDisiPcoCtor0TableKey({22, 13}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(82,
                                                                                                             ConjunctionDisiPco_ctor_0_factory_func_14,
                                                                                                             ConjunctionDisiPcoCtor0TableKey(
                                                                                                                 {30,
                                                                                                                  13},
                                                                                                                 193)),
    ConjunctionDisiPcoCtor0TableCell(83,
                                     ConjunctionDisiPco_ctor_0_factory_func_19,
                                     ConjunctionDisiPcoCtor0TableKey({30, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        84,
        ConjunctionDisiPco_ctor_0_factory_func_69,
        ConjunctionDisiPcoCtor0TableKey({30, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(85,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_74,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 13},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(86,
                                     ConjunctionDisiPco_ctor_0_factory_func_47,
                                     ConjunctionDisiPcoCtor0TableKey({31, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        87,
        ConjunctionDisiPco_ctor_0_factory_func_52,
        ConjunctionDisiPcoCtor0TableKey({31, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(88,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_1,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {11, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(89,
                                     ConjunctionDisiPco_ctor_0_factory_func_6,
                                     ConjunctionDisiPcoCtor0TableKey({11, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        90,
        ConjunctionDisiPco_ctor_0_factory_func_34,
        ConjunctionDisiPcoCtor0TableKey({13, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(91,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_5,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {11, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(92,
                                     ConjunctionDisiPco_ctor_0_factory_func_10,
                                     ConjunctionDisiPcoCtor0TableKey({11, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        93,
        ConjunctionDisiPco_ctor_0_factory_func_38,
        ConjunctionDisiPcoCtor0TableKey({13, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(94,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_43,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(95,
                                     ConjunctionDisiPco_ctor_0_factory_func_93,
                                     ConjunctionDisiPcoCtor0TableKey({13, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        96,
        ConjunctionDisiPco_ctor_0_factory_func_98,
        ConjunctionDisiPcoCtor0TableKey({13, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(97,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_39,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(98,
                                     ConjunctionDisiPco_ctor_0_factory_func_89,
                                     ConjunctionDisiPcoCtor0TableKey({13, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        99,
        ConjunctionDisiPco_ctor_0_factory_func_94,
        ConjunctionDisiPcoCtor0TableKey({13, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(100,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_27,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(101,
                                     ConjunctionDisiPco_ctor_0_factory_func_32,
                                     ConjunctionDisiPcoCtor0TableKey({20, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        102,
        ConjunctionDisiPco_ctor_0_factory_func_82,
        ConjunctionDisiPcoCtor0TableKey({20, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(103,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_87,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(104,
                                     ConjunctionDisiPco_ctor_0_factory_func_23,
                                     ConjunctionDisiPcoCtor0TableKey({20, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        105,
        ConjunctionDisiPco_ctor_0_factory_func_28,
        ConjunctionDisiPcoCtor0TableKey({20, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(106,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_78,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(107,
                                     ConjunctionDisiPco_ctor_0_factory_func_83,
                                     ConjunctionDisiPcoCtor0TableKey({20, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        108,
        ConjunctionDisiPco_ctor_0_factory_func_60,
        ConjunctionDisiPcoCtor0TableKey({22, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(109,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_65,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(110,
                                     ConjunctionDisiPco_ctor_0_factory_func_115,
                                     ConjunctionDisiPcoCtor0TableKey({22, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        111,
        ConjunctionDisiPco_ctor_0_factory_func_120,
        ConjunctionDisiPcoCtor0TableKey({22, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(112,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_56,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(113,
                                     ConjunctionDisiPco_ctor_0_factory_func_61,
                                     ConjunctionDisiPcoCtor0TableKey({22, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        114,
        ConjunctionDisiPco_ctor_0_factory_func_111,
        ConjunctionDisiPcoCtor0TableKey({22, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(115,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_116,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(116,
                                     ConjunctionDisiPco_ctor_0_factory_func_16,
                                     ConjunctionDisiPcoCtor0TableKey({30, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        117,
        ConjunctionDisiPco_ctor_0_factory_func_21,
        ConjunctionDisiPcoCtor0TableKey({30, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(118,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_71,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(119,
                                     ConjunctionDisiPco_ctor_0_factory_func_4,
                                     ConjunctionDisiPcoCtor0TableKey({11, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        120,
        ConjunctionDisiPco_ctor_0_factory_func_9,
        ConjunctionDisiPcoCtor0TableKey({11, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(121,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_37,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(122,
                                     ConjunctionDisiPco_ctor_0_factory_func_42,
                                     ConjunctionDisiPcoCtor0TableKey({13, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        123,
        ConjunctionDisiPco_ctor_0_factory_func_92,
        ConjunctionDisiPcoCtor0TableKey({13, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(124,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_97,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {13, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(125,
                                     ConjunctionDisiPco_ctor_0_factory_func_76,
                                     ConjunctionDisiPcoCtor0TableKey({30, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        126,
        ConjunctionDisiPco_ctor_0_factory_func_12,
        ConjunctionDisiPcoCtor0TableKey({30, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(127,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_17,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(128,
                                     ConjunctionDisiPco_ctor_0_factory_func_26,
                                     ConjunctionDisiPcoCtor0TableKey({20, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        129,
        ConjunctionDisiPco_ctor_0_factory_func_31,
        ConjunctionDisiPcoCtor0TableKey({20, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(130,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_81,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {20, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(131,
                                     ConjunctionDisiPco_ctor_0_factory_func_86,
                                     ConjunctionDisiPcoCtor0TableKey({20, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        132,
        ConjunctionDisiPco_ctor_0_factory_func_59,
        ConjunctionDisiPcoCtor0TableKey({22, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(133,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_64,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {22, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(134,
                                     ConjunctionDisiPco_ctor_0_factory_func_114,
                                     ConjunctionDisiPcoCtor0TableKey({22, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        135,
        ConjunctionDisiPco_ctor_0_factory_func_119,
        ConjunctionDisiPcoCtor0TableKey({22, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(136,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_67,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(137,
                                     ConjunctionDisiPco_ctor_0_factory_func_72,
                                     ConjunctionDisiPcoCtor0TableKey({30, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        138,
        ConjunctionDisiPco_ctor_0_factory_func_15,
        ConjunctionDisiPcoCtor0TableKey({30, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(139,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_20,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {30, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(140,
                                     ConjunctionDisiPco_ctor_0_factory_func_70,
                                     ConjunctionDisiPcoCtor0TableKey({30, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        141,
        ConjunctionDisiPco_ctor_0_factory_func_75,
        ConjunctionDisiPcoCtor0TableKey({30, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(142,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_102,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 13},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(143,
                                     ConjunctionDisiPco_ctor_0_factory_func_107,
                                     ConjunctionDisiPcoCtor0TableKey({31, 13}, 193)), ConjunctionDisiPcoCtor0TableCell(
        144,
        ConjunctionDisiPco_ctor_0_factory_func_49,
        ConjunctionDisiPcoCtor0TableKey({31, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(145,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_54,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 22},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(146,
                                     ConjunctionDisiPco_ctor_0_factory_func_104,
                                     ConjunctionDisiPcoCtor0TableKey({31, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(
        147,
        ConjunctionDisiPco_ctor_0_factory_func_109,
        ConjunctionDisiPcoCtor0TableKey({31, 22}, 193)), ConjunctionDisiPcoCtor0TableCell(148,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_45,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(149,
                                     ConjunctionDisiPco_ctor_0_factory_func_50,
                                     ConjunctionDisiPcoCtor0TableKey({31, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(
        150,
        ConjunctionDisiPco_ctor_0_factory_func_100,
        ConjunctionDisiPcoCtor0TableKey({31, 30}, 193)), ConjunctionDisiPcoCtor0TableCell(151,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_105,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 30},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(152,
                                     ConjunctionDisiPco_ctor_0_factory_func_48,
                                     ConjunctionDisiPcoCtor0TableKey({31, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(
        153,
        ConjunctionDisiPco_ctor_0_factory_func_53,
        ConjunctionDisiPcoCtor0TableKey({31, 31}, 193)), ConjunctionDisiPcoCtor0TableCell(154,
                                                                                          ConjunctionDisiPco_ctor_0_factory_func_103,
                                                                                          ConjunctionDisiPcoCtor0TableKey(
                                                                                              {31, 31},
                                                                                              193)),
    ConjunctionDisiPcoCtor0TableCell(0,
                                     ConjunctionDisiPco_ctor_0_factory_func_108,
                                     ConjunctionDisiPcoCtor0TableKey({31, 31}, 193)),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(),
    ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell(), ConjunctionDisiPcoCtor0TableCell()
};

ManagedPtr<DocIdSetIterator> PcoTargetClassFactoryOfLuceneCyborgSearchQueryConjunctionDisiPco::create(
    ManagedPtr<DocIdSetIterator> param_0,
    ManagedPtr<DocIdSetIterator> param_1,
    ManagedPtr<LVector<util::ManagedPtr<search::DocIdSetIterator>>> param_2,
    ManagedPtr<LinearAllocator> param_3) {
  auto allocator = param_3;

  const auto key = ConjunctionDisiPcoCtor0TableKey({
                                                       param_0 != nullptr ? (((PcoTypeIndexSupport *) param_0)
                                                           ->get_pco_type_index()) : (8),
                                                       param_1 != nullptr ? (((PcoTypeIndexSupport *) param_1)
                                                           ->get_pco_type_index()) : (8)
                                                   },
                                                   193);

  auto factory = PcoTableLookUp::look_up<ConjunctionDisiPcoCtor0TableKey,
                                         ConjunctionDisiPcoCtor0TableCell,
                                         CTOR_0_HASH_TABLE_SIZE>(key, &ConjunctionDisiPco_CTOR_0_TABLE[0]);

  return factory(param_0, param_1, param_2, allocator);
}

}  // namespace lucene::cyborg::pco
