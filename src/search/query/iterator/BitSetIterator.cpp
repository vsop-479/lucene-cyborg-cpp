#include "BitSetIterator.hpp"

using lucene::cyborg::search::query::BitSetIterator;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::BitSet;

//
// BitSetIterator
//

const TypeInfoPtr BitSetIterator::TYPE_INFO = TypeInfo::get_type_info<BitSetIterator>("BitSetIterator");

BitSetIterator::BitSetIterator(ManagedPtr<BitSet> _bits, int64_t _cost)
    : DocIdSetIterator(TYPE_INFO),
      bits(_bits),
      length(_bits->get_length()),
      cost(_cost),
      doc(-1) {
  if (cost < 0) [[unlikely]] {
    // TODO
    throw 13;
  }
}

int32_t BitSetIterator::advance(int32_t target) {
  if (target < length) {
    return doc = bits->next_set_bit(target);
  }
  return doc = DocIdSetIterator::NO_MORE_DOCS;
}
