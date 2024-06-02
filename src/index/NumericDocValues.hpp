#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_NUMERICDOCVALUES_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_NUMERICDOCVALUES_HPP_

#include "DocValuesIterator.hpp"

namespace lucene::cyborg::index {

struct NumericDocValues : public DocValuesIterator {
  virtual int64_t get_long_value() = 0;
};  // NumericDocValues



}  // namespace lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_NUMERICDOCVALUES_HPP_
