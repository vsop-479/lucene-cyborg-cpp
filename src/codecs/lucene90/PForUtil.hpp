#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_PFORUTIL_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_PFORUTIL_HPP_

#include <cstdint>
#include "ForUtil.hpp"
#include "../../store/DataInput.hpp"
#include "../../store/ReadOnlyDataInput.hpp"

namespace lucene::cyborg::codecs::v90 {

class PForUtil {
 public:
  static void skip_freq(store::DataInput<store::ReadOnlyDataInput> &in);

  void decode(store::DataInput<store::ReadOnlyDataInput> &in, int64_t longs[]);

  void decode_and_prefix_sum(store::DataInput<store::ReadOnlyDataInput> &in, const int64_t base, int64_t longs[]);

  static constexpr int32_t MAX_EXCEPTIONS = 7;
  static constexpr int32_t HALF_BLOCK_SIZE = ForUtil::BLOCK_SIZE / 2;

 private:
  void apply_exceptions32(const uint32_t bits_per_value,
                          const int32_t num_exceptions,
                          store::DataInput<store::ReadOnlyDataInput> &in,
                          int64_t longs[]);

  char exception_buff[MAX_EXCEPTIONS * 2];
};  // PForUtil



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_PFORUTIL_HPP_
