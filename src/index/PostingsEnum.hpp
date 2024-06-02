#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_POSTINGSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_POSTINGSENUM_HPP_

#include "../search/DocIdSetIterator.hpp"

namespace lucene::cyborg::index {

class PostingsEnum : public search::DocIdSetIterator {
 public:
  enum Flags {
    NONE = 0,
    FREQS = 1 << 3,
    POSITIONS = (1 << 3) | (1 << 4),
    OFFSETS = (1 << 3) | (1 << 4) | (1 << 5),
    PAYLOADS = (1 << 3) | (1 << 4) | (1 << 6),
    All = (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6)
  };  // Flags

  explicit PostingsEnum(util::TypeInfoPtr _iterator_type_info)
      : search::DocIdSetIterator(_iterator_type_info) {
  }

  virtual int32_t next_doc() = 0;

  virtual int32_t get_freq() {
    // TODO : remove this.
    return 0;
  }

  virtual int64_t get_cost() = 0;
};  // PostingsEnum



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_POSTINGSENUM_HPP_
