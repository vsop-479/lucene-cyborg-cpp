#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_LEAFREADERCONTEXT_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_LEAFREADERCONTEXT_HPP_

#include "../util/PointerTypes.hpp"
#include "SegmentReader.hpp"

namespace lucene::cyborg::index {

struct LeafReaderContext {
  int32_t ord;
  int32_t doc_base;
  util::ManagedPtr<SegmentReader> segment_reader;
};  // LeafReaderContext



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_LEAFREADERCONTEXT_HPP_
