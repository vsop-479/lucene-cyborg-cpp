#! /usr/bin/env python

# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# from fractions import gcd

"""Code generation for ForUtil.java"""

MAX_SPECIALIZED_BITS_PER_VALUE = 24
OUTPUT_FILE = "ForUtil.hpp"
PRIMITIVE_SIZE = [8, 16, 32]
HEADER = """// This file has been automatically generated, DO NOT EDIT

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_FORUTIL_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_FORUTIL_HPP_

#include <cstdint>
#include "../../store/DataInput.hpp"
#include "../../store/ReadOnlyDataInput.hpp"

namespace lucene::cyborg::codecs::v90 {

// Inspired from https://fulmicoton.com/posts/bitpacking/
// Encodes multiple integers in a long to get SIMD-like speedups.
// If bits_per_value <= 8 then we pack 8 ints per long
// else if bits_per_value <= 16 we pack 4 ints per long
// else we pack 2 ints per long
struct ForUtil final {
  static constexpr int32_t BLOCK_SIZE = 128;
  static constexpr int32_t BLOCK_SIZE_LOG2 = 7;

  static void expand8(uint64_t *arr) noexcept {
    for (int32_t i = 0; i < 16; ++i) {
      const uint64_t l = arr[i];
      arr[i] = (l >> 56U);
      arr[16 + i] = (l >> 48U) & 0xFFULL;
      arr[32 + i] = (l >> 40U) & 0xFFULL;
      arr[48 + i] = (l >> 32U) & 0xFFULL;
      arr[64 + i] = (l >> 24U) & 0xFFULL;
      arr[80 + i] = (l >> 16U) & 0xFFULL;
      arr[96 + i] = (l >> 8U) & 0xFFULL;
      arr[112 + i] = l & 0xFFULL;
    }  // End for
  }

  static void expand8_to32(uint64_t *arr) noexcept {
    for (int32_t i = 0; i < 16; ++i) {
      const uint64_t l = arr[i];
      arr[i] = (l >> 24U) & 0x000000FF000000FFULL;
      arr[16 + i] = (l >> 16U) & 0x000000FF000000FFULL;
      arr[32 + i] = (l >> 8U) & 0x000000FF000000FFULL;
      arr[48 + i] = l & 0x000000FF000000FFULL;
    }  // End for
  }

  static void expand16(uint64_t *arr) noexcept {
    for (int32_t i = 0; i < 32; ++i) {
      const uint64_t l = arr[i];
      arr[i] = (l >> 48U) & 0xFFFFULL;
      arr[32 + i] = (l >> 32U) & 0xFFFFULL;
      arr[64 + i] = (l >> 16U) & 0xFFFFULL;
      arr[96 + i] = l & 0xFFFFULL;
    }  // End for
  }

  static void expand16_to32(uint64_t *arr) noexcept {
    for (int32_t i = 0; i < 32; ++i) {
      const uint64_t l = arr[i];
      arr[i] = (l >> 16U) & 0x0000FFFF0000FFFFULL;
      arr[32 + i] = l & 0x0000FFFF0000FFFFULL;
    }  // End for
  }

  static void expand32(uint64_t *arr) noexcept {
    for (int32_t i = 0; i < 64; ++i) {
      const uint64_t l = arr[i];
      arr[i] = l >> 32U;
      arr[64 + i] = l & 0xFFFFFFFFULL;
    }  // End for
  }

  /** Number of bytes required to encode 128 integers of {@code bitsPerValue} bits per value. */
  static constexpr int32_t num_bytes(const uint32_t bits_per_value) noexcept {
    return bits_per_value << (BLOCK_SIZE_LOG2 - 3);
  }

  static void decode_slow(const int32_t bits_per_value,
                          store::DataInput<store::ReadOnlyDataInput>& in,
                          uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t*) in.input.data;
  
    int32_t longs_idx = bulk_shift_longs(in, tmp, longs, bits_per_value);
    int32_t shift = 32 - bits_per_value;
    const int32_t remaining_bits_per_long = shift + bits_per_value;
    const uint64_t mask32_remaining_bits_per_long = MASKS32[remaining_bits_per_long];
    int32_t tmp_idx = 0;
    int32_t remaining_bits = remaining_bits_per_long;
    for (; longs_idx < (BLOCK_SIZE / 2); ++longs_idx) {
      int32_t b = bits_per_value - remaining_bits;
      // Java allows negative shifting
      // From the JLS, section 15.19 (Shift Operators):
      // If the promoted type of the left-hand operand is int,
      // only the five lowest-order bits of the right-hand operand are used as the shift distance.
      // It is as if the right-hand operand were subjected to a bitwise logical AND operator & (15.22.1)
      // with the mask value 0x1f (0b11111). The shift distance actually used is therefore always in the range 0 to 31,
      // inclusive.
      uint64_t l = (tmp[tmp_idx++] & MASKS32[remaining_bits]) << (((uint32_t) b) & 0x1FU);
      while (b >= remaining_bits_per_long) {
        b -= remaining_bits_per_long;
        l |= (tmp[tmp_idx++] & mask32_remaining_bits_per_long) << (((uint32_t) b) & 0x1FU);
      }
      if (b > 0) {
        l |= (tmp[tmp_idx] >> (remaining_bits_per_long - b)) & MASKS32[b];
        remaining_bits = remaining_bits_per_long - b;
      } else {
        remaining_bits = remaining_bits_per_long;
      }
      longs[longs_idx] = l;
    }  // End for
  }

  template <int32_t Count, uint32_t Shift, uint64_t Mask>
  static void shift_longs(const uint64_t* src,
                          uint64_t* dest) noexcept {
    for (int32_t i = 0 ; i < Count ; ++i) {
      *dest++ = (*src++ >> Shift) & Mask;
    }  // End for
  }
"""


def write_shift_longs_per_bpv(f, bpv):
    if bpv == 32:
        f.write("        shift_longs<64, 0, ~0LLU>(src, dest);\n")
        f.write("        return 64;\n")
        return

    shift = 32 - bpv
    num_longs = bpv << 1
    longs_idx = 0
    f.write('        in.skip_bytes(8 * %d);\n' % num_longs)
    while shift >= 0:
        f.write("        shift_longs<%d, %d, MASKS32[%d]>(src, dest + %d);\n" % (num_longs, shift, bpv, longs_idx))
        longs_idx += num_longs
        shift -= bpv
    f.write("        return %d;\n" % longs_idx)


def write_bulk_shift_longs(f):
    f.write('  static int32_t bulk_shift_longs(store::DataInput<store::ReadOnlyDataInput>& in,\n'
            '                                  const uint64_t *src,\n'
            '                                  uint64_t* dest,\n'
            '                                  const uint32_t bits_per_value) noexcept {\n')
    f.write('    switch (bits_per_value) {\n')
    for bpv in range(1, 33):
        f.write('      case %dU:\n' % bpv)
        write_shift_longs_per_bpv(f, bpv)
    f.write('    }\n')
    f.write('  }\n')


def writeRemainderWithSIMDOptimize(bpv, next_primitive, remaining_bits_per_long, o, num_values, f):
    iteration = 1
    num_longs = bpv * num_values / remaining_bits_per_long
    while num_longs % 2 == 0 and num_values % 2 == 0:
        num_longs /= 2
        num_values /= 2
        iteration *= 2

    mask = 'MASK%d_%d' % (next_primitive, remaining_bits_per_long)
    f.write(
        '    for (int32_t iter = 0, tmp_idx = 0, longs_idx = %d; iter < %d; ++iter, tmp_idx += %d, longs_idx += %d) {\n' % (
            o, iteration, num_longs, num_values))
    tmp_idx = 0
    b = bpv
    b -= remaining_bits_per_long
    f.write('      uint64_t l0 = (tmp[tmp_idx + %d] & %s) << %dU;\n' % (tmp_idx, mask, b))
    tmp_idx += 1
    while b >= remaining_bits_per_long:
        b -= remaining_bits_per_long
        f.write('      l0 |= (tmp[tmp_idx + %d] & %s) << %dU;\n' % (tmp_idx, mask, b))
        tmp_idx += 1
    f.write('      longs[longs_idx + 0] = l0;\n')
    f.write('    }\n')


def writeRemainder(bpv, next_primitive, remaining_bits_per_long, o, num_values, f):
    iteration = 1
    num_longs = bpv * num_values / remaining_bits_per_long
    while num_longs % 2 == 0 and num_values % 2 == 0:
        num_longs /= 2
        num_values /= 2
        iteration *= 2
    f.write(
        '    for (int32_t iter = 0, tmp_idx = 0, longs_idx = %d; iter < %d; ++iter, tmp_idx += %d, longs_idx += %d) {\n' % (
            o, iteration, num_longs, num_values))
    remaining_bits = 0
    tmp_idx = 0
    for i in range(int(num_values)):
        b = bpv
        if remaining_bits == 0:
            b -= remaining_bits_per_long
            f.write('      uint64_t l%d = (tmp[tmp_idx + %d] & MASK%d_%d) << %dU;\n' % (
                i, tmp_idx, next_primitive, remaining_bits_per_long, b))
        else:
            b -= remaining_bits
            f.write('      uint64_t l%d = (tmp[tmp_idx + %d] & MASK%d_%d) << %dU;\n' % (
                i, tmp_idx, next_primitive, remaining_bits, b))
        tmp_idx += 1
        while b >= remaining_bits_per_long:
            b -= remaining_bits_per_long
            f.write('      l%d |= (tmp[tmp_idx + %d] & MASK%d_%d) << %dU;\n' % (
                i, tmp_idx, next_primitive, remaining_bits_per_long, b))
            tmp_idx += 1
        if b > 0:
            f.write('      l%d |= (tmp[tmp_idx + %d] >> %dU) & MASK%d_%d;\n' % (
                i, tmp_idx, remaining_bits_per_long - b, next_primitive, b))
            remaining_bits = remaining_bits_per_long - b
        f.write('      longs[longs_idx + %d] = l%d;\n' % (i, i))
    f.write('    }  // End for\n')


def writeDecode(bpv, f):
    next_primitive = 32
    if bpv <= 8:
        next_primitive = 8
    elif bpv <= 16:
        next_primitive = 16
    f.write(
        '  static void decode%d(store::DataInput<store::ReadOnlyDataInput>& in, uint64_t *longs) noexcept {\n' % bpv)
    num_values_per_long = 64 / next_primitive
    if bpv == next_primitive:
        # direct copy
        f.write('    in.read_longs<%d>(longs);\n' % (bpv * 2))
    else:
        f.write('    const auto tmp = (const uint64_t*) in.input.data;\n')
        f.write('    in.skip_bytes(8 * %d);\n' % (bpv * 2))
        o = 0
        shift = next_primitive - bpv
        while shift >= 0:
            f.write('    shift_longs<%d, %d, MASK%d_%d>(tmp, longs + %d);\n' % (bpv * 2, shift, next_primitive, bpv, o))
            o += bpv * 2
            shift -= bpv
        if shift + bpv > 0:
            if bpv % (next_primitive % bpv) == 0:
                writeRemainderWithSIMDOptimize(bpv, next_primitive, shift + bpv, o, 128 / num_values_per_long - o, f)
            else:
                writeRemainder(bpv, next_primitive, shift + bpv, o, 128 / num_values_per_long - o, f)
    f.write('  }\n')


if __name__ == '__main__':
    f = open(OUTPUT_FILE, 'w')
    f.write(HEADER)
    f.write(
        '  static constexpr uint64_t MASKS8[8] = {0LLU, 72340172838076673LLU, 217020518514230019LLU, 506381209866536711LLU, 1085102592571150095LLU, 2242545357980376863LLU, 4557430888798830399LLU, 9187201950435737471LLU};\n')
    f.write(
        '  static constexpr uint64_t MASKS16[16] = {0LLU, 281479271743489LLU, 844437815230467LLU, 1970354902204423LLU, 4222189076152335LLU, 8725857424048159LLU, 17733194119839807LLU, 35747867511423103LLU, 71777214294589695LLU, 143835907860922879LLU, 287953294993589247LLU, 576188069258921983LLU, 1152657617789587455LLU, 2305596714850918399LLU, 4611474908973580287LLU, 9223231297218904063LLU};\n')
    f.write(
        '  static constexpr uint64_t MASKS32[32] = {0LLU, 4294967297LLU, 12884901891LLU, 30064771079LLU, 64424509455LLU, 133143986207LLU, 270582939711LLU, 545460846719LLU, 1095216660735LLU, 2194728288767LLU, 4393751544831LLU, 8791798056959LLU, 17587891081215LLU, 35180077129727LLU, 70364449226751LLU, 140733193420799LLU, 281470681808895LLU, 562945658585087LLU, 1125895612137471LLU, 2251795519242239LLU, 4503595333451775LLU, 9007194961870847LLU, 18014394218708991LLU, 36028792732385279LLU, 72057589759737855LLU, 144115183814443007LLU, 288230371923853311LLU, 576460748142673919LLU, 1152921500580315135LLU, 2305843005455597567LLU, 4611686015206162431LLU, 9223372034707292159LLU};\n')
    f.write('\n')
    write_bulk_shift_longs(f)
    f.write("""
  // mark values in array as final longs to avoid the cost of reading array, arrays should only be
  // used when the idx is a variable
""")
    for primitive_size in PRIMITIVE_SIZE:
        for bpv in range(1, min(MAX_SPECIALIZED_BITS_PER_VALUE + 1, primitive_size)):
            if bpv * 2 != primitive_size or primitive_size == 8:
                f.write('  static constexpr uint64_t MASK%d_%d = MASKS%d[%d];\n' % (
                    primitive_size, bpv, primitive_size, bpv))

    f.write("""
  /** Decode 128 integers into {@code longs}. */
  static void decode(const uint32_t bits_per_value, store::DataInput<store::ReadOnlyDataInput>& in, int64_t *longs) noexcept {
    switch (bits_per_value) {
""")
    for bpv in range(1, MAX_SPECIALIZED_BITS_PER_VALUE + 1):
        next_primitive = 32
        if bpv <= 8:
            next_primitive = 8
        elif bpv <= 16:
            next_primitive = 16
        f.write('      case %dU:\n' % bpv)
        f.write('        decode%d(in, (uint64_t*) longs);\n' % bpv)
        f.write('        expand%d((uint64_t*) longs);\n' % next_primitive)
        f.write('        break;\n')
    f.write('      default:\n')
    f.write('        decode_slow(bits_per_value, in, (uint64_t*) longs);\n')
    f.write('        expand32((uint64_t*) longs);\n')
    f.write('        break;\n')
    f.write('    }\n')
    f.write('  }\n')

    f.write("""
  /**
   * Decodes 128 integers into 64 {@code longs} such that each long contains two values, each
   * represented with 32 bits. Values [0..63] are encoded in the high-order bits of {@code longs}
   * [0..63], and values [64..127] are encoded in the low-order bits of {@code longs} [0..63]. This
   * representation may allow subsequent operations to be performed on two values at a time.
   */
  static void decode_to32(const uint32_t bits_per_value,
                          store::DataInput<store::ReadOnlyDataInput>& in,
                          int64_t *longs) {
    switch (bits_per_value) {
""")
    for bpv in range(1, MAX_SPECIALIZED_BITS_PER_VALUE + 1):
        next_primitive = 32
        if bpv <= 8:
            next_primitive = 8
        elif bpv <= 16:
            next_primitive = 16
        f.write('      case %d:\n' % bpv)
        f.write('        decode%d(in, (uint64_t*) longs);\n' % bpv)
        if next_primitive <= 16:
            f.write('        expand%d_to32((uint64_t*) longs);\n' % next_primitive)
        f.write('        break;\n')
    f.write('      default:\n')
    f.write('        decode_slow(bits_per_value, in, (uint64_t*) longs);\n')
    f.write('        break;\n')
    f.write('    }\n')
    f.write('  }\n')

    f.write('\n')
    for i in range(1, MAX_SPECIALIZED_BITS_PER_VALUE + 1):
        writeDecode(i, f)
        if i < MAX_SPECIALIZED_BITS_PER_VALUE:
            f.write('\n')

    f.write('};  // ForUtil\n')
    f.write("""
}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_FORUTIL_HPP_""")
