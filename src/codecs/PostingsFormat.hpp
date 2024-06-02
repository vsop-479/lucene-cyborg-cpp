#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_POSTINGSFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_POSTINGSFORMAT_HPP_

#include "EffectiveCodecs.hpp"

namespace lucene::cyborg::codecs {

template<typename PrevCodecType, typename CurrentCodecType>
class PostingsFormatForTwoVersions {

};  // PostingsFormatForTwoVersions

using PostingsFormat = PostingsFormatForTwoVersions<PreviousCodec, CurrentCodec>;

}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_POSTINGSFORMAT_HPP_
