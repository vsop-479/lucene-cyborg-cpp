#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUESITERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUESITERATOR_HPP_

#include "../search/DocIdSetIterator.hpp"

namespace lucene::cyborg::index {

struct DocValuesIterator : public search::DocIdSetIterator {
  virtual bool advance_exact(int32_t target) = 0;
};  // DocValuesIterator



}  // namespace lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUESITERATOR_HPP_
