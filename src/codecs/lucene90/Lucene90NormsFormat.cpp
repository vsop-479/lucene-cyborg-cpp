#include "Lucene90NormsFormat.hpp"

using lucene::cyborg::codecs::v90::Lucene90NormsFormat;
using lucene::cyborg::codecs::v90::Lucene90NormsProducerPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::index::SegmentReadState;

//
// Lucene90NormsFormat
//

Lucene90NormsProducerPtr Lucene90NormsFormat::create_norms_producer(ReadOnlyPtr<SegmentReadState> read_state) {
  return std::make_shared<Lucene90NormsProducer>(read_state);
}
