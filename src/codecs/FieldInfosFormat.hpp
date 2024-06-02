#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_FIELDINFOSFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_FIELDINFOSFORMAT_HPP_

#include "EffectiveCodecs.hpp"

namespace lucene::cyborg::codecs {

template<typename PrevCodecType, typename CurrentCodecType>
class FieldInfosFormatForTwoVersions {

};  // FieldInfosFormat

using FieldInfosFormat = FieldInfosFormatForTwoVersions<PreviousCodec, CurrentCodec>;

}  // lucene::cyborg::codecs


#endif //LUCENE_CYBORG_CPP_SRC_CODECS_FIELDINFOSFORMAT_HPP_
