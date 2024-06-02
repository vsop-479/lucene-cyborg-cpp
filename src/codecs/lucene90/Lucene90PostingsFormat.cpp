#include "Lucene90PostingsFormat.hpp"
#include "Lucene90PostingsReader.hpp"

using lucene::cyborg::codecs::v90::block_tree::Lucene90BlockTreeTermsReaderPtr;
using lucene::cyborg::codecs::v90::Lucene90PostingsFormat;
using lucene::cyborg::codecs::v90::block_tree::Lucene90BlockTreeTermsReader;
using lucene::cyborg::codecs::v90::Lucene90PostingsReader;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::SegmentReadState;

//
// Lucene90PostingsFormat
//

const std::string Lucene90PostingsFormat::NAME = "Lucene90";
const std::string Lucene90PostingsFormat::DOC_EXTENSION = "doc";
const std::string Lucene90PostingsFormat::POS_EXTENSION = "pos";
const std::string Lucene90PostingsFormat::PAY_EXTENSION = "pay";

Lucene90BlockTreeTermsReaderPtr Lucene90PostingsFormat::fields_producer(
    ReadOnlyPtr<SegmentReadState> state) {
  return std::make_shared<Lucene90BlockTreeTermsReader>(
      std::make_shared<Lucene90PostingsReader>(state),
      state
  );
}
