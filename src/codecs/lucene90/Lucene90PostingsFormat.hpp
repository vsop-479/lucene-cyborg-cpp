#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSFORMAT_HPP_

#include <memory>
#include "../../index/SegmentReadState.hpp"
#include "block_tree/Lucene90BlockTreeTermsReader.hpp"

namespace lucene::cyborg::codecs::v90 {

struct Lucene90PostingsFormat final {
  using FieldsProducerPtr = block_tree::Lucene90BlockTreeTermsReaderPtr;

  static const std::string NAME;
  static const std::string DOC_EXTENSION;
  static const std::string POS_EXTENSION;
  static const std::string PAY_EXTENSION;

  block_tree::Lucene90BlockTreeTermsReaderPtr fields_producer(util::ReadOnlyPtr<index::SegmentReadState> state);
};  // Lucene90PostingsFormat



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSFORMAT_HPP_
