#include "LeafSimScorer.hpp"

using lucene::cyborg::index::SegmentReader;
using lucene::cyborg::search::LeafSimScorer;
using lucene::cyborg::search::SimScorer;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

//
// LeafSimScorer
//

LeafSimScorer::LeafSimScorer(ManagedPtr<LinearAllocator> _allocator,
                             ManagedPtr<SimScorer> _scorer,
                             ManagedPtr<SegmentReader> _segment_reader,
                             const std::string_view &_field)
    : scorer(_scorer),
      norms(_segment_reader->get_norm_values(_allocator, _field)) {
  if (_scorer == nullptr) [[unlikely]] {
    // TODO
    throw 13;
  }
}

float LeafSimScorer::get_score(int32_t doc, float freq) {
  return scorer->score(freq, get_norm_value(doc));
}

int64_t LeafSimScorer::get_norm_value(int32_t doc) const {
  if (norms) {
    const bool found = norms->advance_exact(doc);
    assert(found);
    return norms->get_long_value();
  } else {
    return 1L; // default norm
  }
}
