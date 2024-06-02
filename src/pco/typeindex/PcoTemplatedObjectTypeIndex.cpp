#include "PcoTemplatedObjectTypeIndex.hpp"
#include "../../search/ImpactsDISIPco.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../codecs/lucene90/Lucene90PostingsEnum.hpp"
#include "../../search/query/BlockMaxConjunctionScorerIteratorPco.hpp"
#include "../../codecs/lucene90/Lucene90ImpactsEnum.hpp"

// PCO_INDEX definitions
// Parameter interface: [lucene::cyborg::search::DocIdSetIterator]
template<> uint64_t
    lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>::
    PCO_INDEX = 12;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>::PCO_INDEX =
    13;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>::
    PCO_INDEX = 14;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>::
    PCO_INDEX = 15;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>::PCO_INDEX = 16;
template<> uint64_t
    lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>>::
    PCO_INDEX = 17;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>>::
    PCO_INDEX = 18;
template<> uint64_t lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::search::DocIdSetIterator>::PCO_INDEX = 19;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS>::PCO_INDEX = 20;
template<> uint64_t
    lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>>::
    PCO_INDEX = 21;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>::
    PCO_INDEX = 22;
template<> uint64_t
    lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>::
    PCO_INDEX = 23;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::search::DocIdSetIterator>::PCO_INDEX =
    24;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>::PCO_INDEX = 25;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::NONE>>::
    PCO_INDEX = 26;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>::PCO_INDEX =
    27;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS>::
    PCO_INDEX = 28;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS>>::
    PCO_INDEX = 29;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::NONE>::PCO_INDEX = 30;
template<> uint64_t
    lucene::cyborg::codecs::v90::Lucene90PostingsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>::
    PCO_INDEX = 31;
template<> uint64_t
    lucene::cyborg::search::BlockMaxConjunctionScorerIteratorPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>::
    PCO_INDEX = 32;
template<> uint64_t
    lucene::cyborg::search::ImpactsDISIPco<lucene::cyborg::codecs::v90::Lucene90ImpactsEnum<lucene::cyborg::index::IndexOptions::DOCS_AND_FREQS>>::
    PCO_INDEX = 33;

bool lucene::cyborg::pco::PcoTemplatedObjectTypeIndex::assign_type_index() {
  return true;
}
