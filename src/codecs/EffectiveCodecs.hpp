#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_EFFECTIVECODECS_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_EFFECTIVECODECS_HPP_

#include "./lucene95/Lucene95Codec.hpp"

namespace lucene::cyborg::codecs {

using CurrentCodec = v95::Lucene95Codec;
using CurrentPostingsFormat = CurrentCodec::PostingsFormat;
using CurrentFieldInfosFormat = CurrentCodec::FieldInfosFormat;

using PreviousCodec = v95::Lucene95Codec;
using PreviousPostingsFormat = PreviousCodec::PostingsFormat;
using PreviousFieldInfosFormat = PreviousCodec::FieldInfosFormat;

}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_EFFECTIVECODECS_HPP_
