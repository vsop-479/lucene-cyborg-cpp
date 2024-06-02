#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFSIMSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFSIMSCORER_HPP_

#include "../util/PointerTypes.hpp"
#include "Similarity.hpp"
#include "../index/SegmentReader.hpp"
#include "../index/NumericDocValues.hpp"

namespace lucene::cyborg::search {

class LeafSimScorer {
 public:
  LeafSimScorer(util::ManagedPtr<util::LinearAllocator> allocator,
                util::ManagedPtr<SimScorer> scorer,
                util::ManagedPtr<index::SegmentReader> segment_reader,
                const std::string_view &field);

  float get_score(int32_t doc, float freq);

  // TODO : Explain

  util::ManagedPtr<SimScorer> scorer;
  util::ManagedPtr<index::NumericDocValues> norms;

 private:
  int64_t get_norm_value(int32_t doc) const;
};  // LeafSimScorer



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFSIMSCORER_HPP_
