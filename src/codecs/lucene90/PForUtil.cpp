#include <array>
#include "PForUtil.hpp"

using lucene::cyborg::codecs::v90::PForUtil;
using lucene::cyborg::codecs::v90::ForUtil;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;

static const std::array<int64_t, ForUtil::BLOCK_SIZE> IDENTITY_PLUS_ONE = []() {
  std::array<int64_t, ForUtil::BLOCK_SIZE> ret{};
  for (int32_t i = 0; i < ForUtil::BLOCK_SIZE; ++i) {
    ret[i] = i + 1;
  }
  return ret;
}();

static void prefix_sum_of_ones(int64_t *longs, const int64_t base) noexcept {
  for (int32_t i = 0; i < ForUtil::BLOCK_SIZE; ++i) {
    longs[i] = IDENTITY_PLUS_ONE[i] + base;
  }
}

static void prefix_sum_of(int64_t longs[], const int64_t base, const int64_t val) noexcept {
  const int64_t val_base = val + base;
  for (int32_t i = 0; i < ForUtil::BLOCK_SIZE; ++i) {
    longs[i] = i * val + val_base;
  }
}

static void inner_prefix_sum32(int64_t longs[]) noexcept {
  longs[1] += longs[0];
  longs[2] += longs[1];
  longs[3] += longs[2];
  longs[4] += longs[3];
  longs[5] += longs[4];
  longs[6] += longs[5];
  longs[7] += longs[6];
  longs[8] += longs[7];
  longs[9] += longs[8];
  longs[10] += longs[9];
  longs[11] += longs[10];
  longs[12] += longs[11];
  longs[13] += longs[12];
  longs[14] += longs[13];
  longs[15] += longs[14];
  longs[16] += longs[15];
  longs[17] += longs[16];
  longs[18] += longs[17];
  longs[19] += longs[18];
  longs[20] += longs[19];
  longs[21] += longs[20];
  longs[22] += longs[21];
  longs[23] += longs[22];
  longs[24] += longs[23];
  longs[25] += longs[24];
  longs[26] += longs[25];
  longs[27] += longs[26];
  longs[28] += longs[27];
  longs[29] += longs[28];
  longs[30] += longs[29];
  longs[31] += longs[30];
  longs[32] += longs[31];
  longs[33] += longs[32];
  longs[34] += longs[33];
  longs[35] += longs[34];
  longs[36] += longs[35];
  longs[37] += longs[36];
  longs[38] += longs[37];
  longs[39] += longs[38];
  longs[40] += longs[39];
  longs[41] += longs[40];
  longs[42] += longs[41];
  longs[43] += longs[42];
  longs[44] += longs[43];
  longs[45] += longs[44];
  longs[46] += longs[45];
  longs[47] += longs[46];
  longs[48] += longs[47];
  longs[49] += longs[48];
  longs[50] += longs[49];
  longs[51] += longs[50];
  longs[52] += longs[51];
  longs[53] += longs[52];
  longs[54] += longs[53];
  longs[55] += longs[54];
  longs[56] += longs[55];
  longs[57] += longs[56];
  longs[58] += longs[57];
  longs[59] += longs[58];
  longs[60] += longs[59];
  longs[61] += longs[60];
  longs[62] += longs[61];
  longs[63] += longs[62];
}

static void expand32(int64_t longs[]) noexcept {
  for (int32_t i = 0; i < 64; ++i) {
    const uint64_t l = longs[i];
    longs[i] = l >> 32U;
    longs[64 + i] = l & 0xFFFFFFFFLLU;
  }
}

static void prefix_sum32(int64_t longs[], const int64_t base) {
  longs[0] += base << 32U;
  inner_prefix_sum32(longs);
  expand32(longs);
  const auto l = longs[PForUtil::HALF_BLOCK_SIZE - 1];
  for (int32_t i = PForUtil::HALF_BLOCK_SIZE; i < ForUtil::BLOCK_SIZE; ++i) {
    longs[i] += l;
  }
}

static void fill_same_value32(int64_t longs[], const uint64_t val) noexcept {
  const int64_t token = val << 32U | val;
  for (int32_t i = 0; i < PForUtil::HALF_BLOCK_SIZE; ++i) {
    longs[i] = token;
  }
}



//
// PForUtil
//

void PForUtil::decode(DataInput<ReadOnlyDataInput> &in,
                      int64_t *longs) {
  const uint32_t token = in.read_byte();
  const uint32_t bits_per_value = token & 0x1FU;
  const uint32_t num_exceptions = token >> 5U;
  if (bits_per_value == 0) {
    const int64_t value = in.read_vlong();
    for (int32_t i = 0; i < ForUtil::BLOCK_SIZE; ++i) {
      longs[i] = value;
    }
  } else {
    ForUtil::decode(bits_per_value, in, longs);
  }  // End if

  for (int32_t i = 0; i < num_exceptions; ++i) {
    const auto index = in.read_byte();
    longs[index] |= in.read_byte() << bits_per_value;
  }  // End for
}

void PForUtil::skip_freq(DataInput<ReadOnlyDataInput> &in) {
  const uint32_t token = in.read_byte();
  const uint32_t bits_per_value = token & 0x1FU;
  const uint32_t num_exceptions = token >> 5U;
  if (bits_per_value != 0) {
    in.skip_bytes(ForUtil::num_bytes(bits_per_value) + (num_exceptions << 1U));
  } else {
    in.skip_variant_integer();
    in.skip_bytes(num_exceptions << 1U);
  }  // End if
}

void PForUtil::decode_and_prefix_sum(DataInput<ReadOnlyDataInput> &in,
                                     const int64_t base,
                                     int64_t *longs) {
  const uint32_t token = in.read_byte();
  const uint32_t bits_per_value = token & 0x1FU;
  const uint32_t num_exceptions = token >> 5U;
  if (num_exceptions == 0) {
    // when there are no exceptions to apply, we can be a bit more efficient with our decoding
    if (bits_per_value != 0) {
      // decode the deltas then apply the prefix sum logic
      ForUtil::decode_to32(bits_per_value, in, longs);
      prefix_sum32(longs, base);
    } else {
      // a bpv of zero indicates all delta values are the same
      const int64_t val = in.read_vlong();
      if (val == 1) {
        // this will often be the common case when working with doc IDs, so we special-case it to
        // be slightly more efficient
        prefix_sum_of_ones(longs, base);
      } else {
        prefix_sum_of(longs, base, val);
      }
    }
  } else {
    // pack two values per long so we can apply prefixes two-at-a-time
    if (bits_per_value != 0) {
      ForUtil::decode_to32(bits_per_value, in, longs);
    } else {
      fill_same_value32(longs, in.read_vlong());
    }
    apply_exceptions32(bits_per_value, num_exceptions, in, longs);
    prefix_sum32(longs, base);
  }  // End if
}

void PForUtil::apply_exceptions32(const uint32_t bits_per_value,
                                  const int32_t num_exceptions,
                                  DataInput<ReadOnlyDataInput> &in,
                                  int64_t longs[]) {
  auto exception_buff = in.input.data;
  in.skip_bytes(num_exceptions * 2);
  for (int32_t i = 0; i < num_exceptions; ++i) {
    const uint32_t exception_pos = exception_buff[i * 2];
    const uint64_t exception = exception_buff[i * 2 + 1];
    // note that we pack two values per long, so the index is [0..63] for 128 values
    const int32_t idx = exception_pos & 0x3FU; // mod 64
    // we need to shift by 1) the bpv, and 2) 32 for positions [0..63] (and no 32 shift for
    // [64..127])
    const uint32_t shift = bits_per_value + ((1U ^ (exception_pos >> 6U)) << 5U);
    longs[idx] |= exception << shift;
  }  // End for
}
