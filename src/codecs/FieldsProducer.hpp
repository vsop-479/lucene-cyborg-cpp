#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_FIELDSPRODUCER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_FIELDSPRODUCER_HPP_

#include "../index/Fields.hpp"

namespace lucene::cyborg::codecs {

class FieldsProducer : public index::Fields {
 public:
  virtual void close() = 0;

  virtual void check_integrity() = 0;
}; // FieldsProducer



}  // lucene::cyborg::codecs


#endif //LUCENE_CYBORG_CPP_SRC_CODECS_FIELDSPRODUCER_HPP_
