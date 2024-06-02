#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_BITSET_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_BITSET_HPP_

#include "Bits.hpp"
#include "PointerTypes.hpp"
#include "../search/DocIdSetIterator.hpp"

namespace lucene::cyborg::util {

class BitSet : public Bits {
 public:
  virtual void clear_all() {
    clear_range(0, get_length());
  }

  virtual void set(int32_t index) = 0;

  virtual bool get_and_set(int32_t index) = 0;

  virtual void clear(int32_t index) = 0;

  virtual void clear_range(int32_t start_index, int32_t end_index) = 0;

  virtual int32_t get_approximate_cardinality() = 0;

  virtual int32_t prev_set_bit(int32_t index) = 0;

  virtual int32_t next_set_bit(int32_t index) = 0;

  void or_operator(ManagedPtr<search::DocIdSetIterator> iterator);

 protected:
  void check_unpositioned(ManagedPtr<search::DocIdSetIterator> iterator);
};  // BitSet



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_BITSET_HPP_
