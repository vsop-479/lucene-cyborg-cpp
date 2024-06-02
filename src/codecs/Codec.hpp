#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_CODEC_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_CODEC_HPP_

#include "EffectiveCodecs.hpp"
#include "PostingsFormat.hpp"

namespace lucene::cyborg::codecs {

class Codec {
  PostingsFormat postingsFormat() {
    return {};
  }
};  // CodecForTwoVersions



}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_CODEC_HPP_
