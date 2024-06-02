#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE95_LUCENE95CODEC_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE95_LUCENE95CODEC_HPP_

#include "../lucene90/Lucene90PostingsFormat.hpp"
#include "../lucene90/Lucene90NormsFormat.hpp"
#include "../lucene90/Lucene90SegmentInfoFormat.hpp"
#include "../lucene94/Lucene94FieldInfosFormat.hpp"
#include "../PerFieldPostingsFormatAdaptor.hpp"

namespace lucene::cyborg::codecs::v95 {

struct Lucene95Codec {
  using NormsFormat = v90::Lucene90NormsFormat;

  using PostingsFormat = PerFieldPostingsFormatAdaptor<v90::Lucene90PostingsFormat>;

  using SegmentInfoFormat = v90::Lucene90SegmentInfoFormat;

  using FieldInfosFormat = v94::Lucene94FieldInfosFormat;

  static const std::string CODE_NAME;
};  // Lucene95Codec



}  // lucene::cyborg::codecs::v95

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE95_LUCENE95CODEC_HPP_
