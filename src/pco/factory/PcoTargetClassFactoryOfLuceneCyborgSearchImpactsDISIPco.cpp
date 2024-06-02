#include "PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco.hpp"
#include "PcoTableModel.hpp"
#include "../../codecs/lucene90/Lucene90ImpactsEnum.hpp"

using lucene::cyborg::codecs::v90::Lucene90ImpactsEnum;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::ImpactsDISIPco;
using lucene::cyborg::search::ImpactsDISIPcoBase;
using lucene::cyborg::search::MaxScoreCache;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

namespace lucene::cyborg::pco {

//
// Constructor-0, Suffix=[]
// Target type: [lucene::cyborg::search::ImpactsDISIPco]
// Target interface type: [lucene::cyborg::search::ImpactsDISIPcoBase]
//

constexpr int CTOR_0_HASH_TABLE_SIZE = 13;
using ImpactsDISIPcoCtor0TableKey = PcoTableKey<1>;
using ImpactsDISIPcoCtor0FactoryFunction = ManagedPtr<ImpactsDISIPcoBase> (*)(ManagedPtr<DocIdSetIterator>,
                                                                              ManagedPtr<MaxScoreCache>,
                                                                              ManagedPtr<LinearAllocator>);

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_0(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>>(((DocIdSetIterator *) param_0),
                                                                                  param_1);
}

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_1(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE> *) param_0),
          param_1);
}

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_2(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS> *) param_0),
          param_1);
}

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_3(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS> *) param_0),
          param_1);
}

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_4(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS> *) param_0),
          param_1);
}

ManagedPtr<ImpactsDISIPcoBase> ImpactsDISIPco_ctor_0_factory_func_5(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2
) {
  auto allocator = param_2;

  return allocator
      ->allocate_object<ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>>(
          ((Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS> *) param_0),
          param_1);
}

using ImpactsDISIPcoCtor0TableCell = PcoTableCell<ImpactsDISIPcoCtor0FactoryFunction, ImpactsDISIPcoCtor0TableKey>;
ImpactsDISIPcoCtor0TableCell ImpactsDISIPco_CTOR_0_TABLE[CTOR_0_HASH_TABLE_SIZE] = {
    ImpactsDISIPcoCtor0TableCell(1, ImpactsDISIPco_ctor_0_factory_func_0, ImpactsDISIPcoCtor0TableKey({11}, 10)),
    ImpactsDISIPcoCtor0TableCell(3, ImpactsDISIPco_ctor_0_factory_func_2, ImpactsDISIPcoCtor0TableKey({25}, 10)),
    ImpactsDISIPcoCtor0TableCell(),
    ImpactsDISIPcoCtor0TableCell(4, ImpactsDISIPco_ctor_0_factory_func_3, ImpactsDISIPcoCtor0TableKey({27}, 10)),
    ImpactsDISIPcoCtor0TableCell(5, ImpactsDISIPco_ctor_0_factory_func_4, ImpactsDISIPcoCtor0TableKey({15}, 10)),
    ImpactsDISIPcoCtor0TableCell(6, ImpactsDISIPco_ctor_0_factory_func_1, ImpactsDISIPcoCtor0TableKey({16}, 10)),
    ImpactsDISIPcoCtor0TableCell(0, ImpactsDISIPco_ctor_0_factory_func_5, ImpactsDISIPcoCtor0TableKey({28}, 10)),
    ImpactsDISIPcoCtor0TableCell(), ImpactsDISIPcoCtor0TableCell(), ImpactsDISIPcoCtor0TableCell(),
    ImpactsDISIPcoCtor0TableCell(), ImpactsDISIPcoCtor0TableCell(), ImpactsDISIPcoCtor0TableCell()
};

ManagedPtr<ImpactsDISIPcoBase> PcoTargetClassFactoryOfLuceneCyborgSearchImpactsDISIPco::create(
    ManagedPtr<DocIdSetIterator> param_0, ManagedPtr<MaxScoreCache> param_1, ManagedPtr<LinearAllocator> param_2) {
  auto allocator = param_2;

  const auto key = ImpactsDISIPcoCtor0TableKey({
                                                   param_0 != nullptr ? (((PcoTypeIndexSupport *) param_0)
                                                       ->get_pco_type_index()) : (8)
                                               },
                                               10);

  auto factory =
      PcoTableLookUp::look_up<ImpactsDISIPcoCtor0TableKey, ImpactsDISIPcoCtor0TableCell, CTOR_0_HASH_TABLE_SIZE>(key,
                                                                                                                 &ImpactsDISIPco_CTOR_0_TABLE[0]);

  return factory(param_0, param_1, allocator);
}

}  // namespace lucene::cyborg::pco
