#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSENUM_HPP_

#include "PostingsEnum.hpp"
#include "ImpactsSource.hpp"

namespace lucene::cyborg::index {

struct ImpactsEnum : public PostingsEnum, public ImpactsSource {
  explicit ImpactsEnum(util::TypeInfoPtr _iterator_type_info)
      : PostingsEnum(_iterator_type_info),
        ImpactsSource() {
  }
};  // ImpactsEnum



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTSENUM_HPP_
