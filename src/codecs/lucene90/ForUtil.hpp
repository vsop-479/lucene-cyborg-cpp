// This file has been automatically generated, DO NOT EDIT

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

  // Number of bytes required to encode 128 integers of `bitsPerValue` bits per value.
  static constexpr int32_t num_bytes(const uint32_t bits_per_value) noexcept {
    return bits_per_value << (BLOCK_SIZE_LOG2 - 3);
  }

  static void decode_slow(const int32_t bits_per_value,
                          store::DataInput<store::ReadOnlyDataInput> &in,
                          uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;

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

  template<int32_t Count, uint32_t Shift, uint64_t Mask>
  static void shift_longs(const uint64_t *src,
                          uint64_t *dest) noexcept {
    for (int32_t i = 0; i < Count; ++i) {
      *dest++ = (*src++ >> Shift) & Mask;
    }  // End for
  }
  static constexpr uint64_t MASKS8[8] =
      {0LLU, 72340172838076673LLU, 217020518514230019LLU, 506381209866536711LLU, 1085102592571150095LLU,
       2242545357980376863LLU, 4557430888798830399LLU, 9187201950435737471LLU};
  static constexpr uint64_t MASKS16[16] =
      {0LLU, 281479271743489LLU, 844437815230467LLU, 1970354902204423LLU, 4222189076152335LLU, 8725857424048159LLU,
       17733194119839807LLU, 35747867511423103LLU, 71777214294589695LLU, 143835907860922879LLU, 287953294993589247LLU,
       576188069258921983LLU, 1152657617789587455LLU, 2305596714850918399LLU, 4611474908973580287LLU,
       9223231297218904063LLU};
  static constexpr uint64_t MASKS32[32] =
      {0LLU, 4294967297LLU, 12884901891LLU, 30064771079LLU, 64424509455LLU, 133143986207LLU, 270582939711LLU,
       545460846719LLU, 1095216660735LLU, 2194728288767LLU, 4393751544831LLU, 8791798056959LLU, 17587891081215LLU,
       35180077129727LLU, 70364449226751LLU, 140733193420799LLU, 281470681808895LLU, 562945658585087LLU,
       1125895612137471LLU, 2251795519242239LLU, 4503595333451775LLU, 9007194961870847LLU, 18014394218708991LLU,
       36028792732385279LLU, 72057589759737855LLU, 144115183814443007LLU, 288230371923853311LLU, 576460748142673919LLU,
       1152921500580315135LLU, 2305843005455597567LLU, 4611686015206162431LLU, 9223372034707292159LLU};

  static int32_t bulk_shift_longs(store::DataInput<store::ReadOnlyDataInput> &in,
                                  const uint64_t *src,
                                  uint64_t *dest,
                                  const uint32_t bits_per_value) noexcept {
    switch (bits_per_value) {
      case 1U:in.skip_bytes(8 * 2);
        shift_longs<2, 31, MASKS32[1]>(src, dest + 0);
        shift_longs<2, 30, MASKS32[1]>(src, dest + 2);
        shift_longs<2, 29, MASKS32[1]>(src, dest + 4);
        shift_longs<2, 28, MASKS32[1]>(src, dest + 6);
        shift_longs<2, 27, MASKS32[1]>(src, dest + 8);
        shift_longs<2, 26, MASKS32[1]>(src, dest + 10);
        shift_longs<2, 25, MASKS32[1]>(src, dest + 12);
        shift_longs<2, 24, MASKS32[1]>(src, dest + 14);
        shift_longs<2, 23, MASKS32[1]>(src, dest + 16);
        shift_longs<2, 22, MASKS32[1]>(src, dest + 18);
        shift_longs<2, 21, MASKS32[1]>(src, dest + 20);
        shift_longs<2, 20, MASKS32[1]>(src, dest + 22);
        shift_longs<2, 19, MASKS32[1]>(src, dest + 24);
        shift_longs<2, 18, MASKS32[1]>(src, dest + 26);
        shift_longs<2, 17, MASKS32[1]>(src, dest + 28);
        shift_longs<2, 16, MASKS32[1]>(src, dest + 30);
        shift_longs<2, 15, MASKS32[1]>(src, dest + 32);
        shift_longs<2, 14, MASKS32[1]>(src, dest + 34);
        shift_longs<2, 13, MASKS32[1]>(src, dest + 36);
        shift_longs<2, 12, MASKS32[1]>(src, dest + 38);
        shift_longs<2, 11, MASKS32[1]>(src, dest + 40);
        shift_longs<2, 10, MASKS32[1]>(src, dest + 42);
        shift_longs<2, 9, MASKS32[1]>(src, dest + 44);
        shift_longs<2, 8, MASKS32[1]>(src, dest + 46);
        shift_longs<2, 7, MASKS32[1]>(src, dest + 48);
        shift_longs<2, 6, MASKS32[1]>(src, dest + 50);
        shift_longs<2, 5, MASKS32[1]>(src, dest + 52);
        shift_longs<2, 4, MASKS32[1]>(src, dest + 54);
        shift_longs<2, 3, MASKS32[1]>(src, dest + 56);
        shift_longs<2, 2, MASKS32[1]>(src, dest + 58);
        shift_longs<2, 1, MASKS32[1]>(src, dest + 60);
        shift_longs<2, 0, MASKS32[1]>(src, dest + 62);
        return 64;
      case 2U:in.skip_bytes(8 * 4);
        shift_longs<4, 30, MASKS32[2]>(src, dest + 0);
        shift_longs<4, 28, MASKS32[2]>(src, dest + 4);
        shift_longs<4, 26, MASKS32[2]>(src, dest + 8);
        shift_longs<4, 24, MASKS32[2]>(src, dest + 12);
        shift_longs<4, 22, MASKS32[2]>(src, dest + 16);
        shift_longs<4, 20, MASKS32[2]>(src, dest + 20);
        shift_longs<4, 18, MASKS32[2]>(src, dest + 24);
        shift_longs<4, 16, MASKS32[2]>(src, dest + 28);
        shift_longs<4, 14, MASKS32[2]>(src, dest + 32);
        shift_longs<4, 12, MASKS32[2]>(src, dest + 36);
        shift_longs<4, 10, MASKS32[2]>(src, dest + 40);
        shift_longs<4, 8, MASKS32[2]>(src, dest + 44);
        shift_longs<4, 6, MASKS32[2]>(src, dest + 48);
        shift_longs<4, 4, MASKS32[2]>(src, dest + 52);
        shift_longs<4, 2, MASKS32[2]>(src, dest + 56);
        shift_longs<4, 0, MASKS32[2]>(src, dest + 60);
        return 64;
      case 3U:in.skip_bytes(8 * 6);
        shift_longs<6, 29, MASKS32[3]>(src, dest + 0);
        shift_longs<6, 26, MASKS32[3]>(src, dest + 6);
        shift_longs<6, 23, MASKS32[3]>(src, dest + 12);
        shift_longs<6, 20, MASKS32[3]>(src, dest + 18);
        shift_longs<6, 17, MASKS32[3]>(src, dest + 24);
        shift_longs<6, 14, MASKS32[3]>(src, dest + 30);
        shift_longs<6, 11, MASKS32[3]>(src, dest + 36);
        shift_longs<6, 8, MASKS32[3]>(src, dest + 42);
        shift_longs<6, 5, MASKS32[3]>(src, dest + 48);
        shift_longs<6, 2, MASKS32[3]>(src, dest + 54);
        return 60;
      case 4U:in.skip_bytes(8 * 8);
        shift_longs<8, 28, MASKS32[4]>(src, dest + 0);
        shift_longs<8, 24, MASKS32[4]>(src, dest + 8);
        shift_longs<8, 20, MASKS32[4]>(src, dest + 16);
        shift_longs<8, 16, MASKS32[4]>(src, dest + 24);
        shift_longs<8, 12, MASKS32[4]>(src, dest + 32);
        shift_longs<8, 8, MASKS32[4]>(src, dest + 40);
        shift_longs<8, 4, MASKS32[4]>(src, dest + 48);
        shift_longs<8, 0, MASKS32[4]>(src, dest + 56);
        return 64;
      case 5U:in.skip_bytes(8 * 10);
        shift_longs<10, 27, MASKS32[5]>(src, dest + 0);
        shift_longs<10, 22, MASKS32[5]>(src, dest + 10);
        shift_longs<10, 17, MASKS32[5]>(src, dest + 20);
        shift_longs<10, 12, MASKS32[5]>(src, dest + 30);
        shift_longs<10, 7, MASKS32[5]>(src, dest + 40);
        shift_longs<10, 2, MASKS32[5]>(src, dest + 50);
        return 60;
      case 6U:in.skip_bytes(8 * 12);
        shift_longs<12, 26, MASKS32[6]>(src, dest + 0);
        shift_longs<12, 20, MASKS32[6]>(src, dest + 12);
        shift_longs<12, 14, MASKS32[6]>(src, dest + 24);
        shift_longs<12, 8, MASKS32[6]>(src, dest + 36);
        shift_longs<12, 2, MASKS32[6]>(src, dest + 48);
        return 60;
      case 7U:in.skip_bytes(8 * 14);
        shift_longs<14, 25, MASKS32[7]>(src, dest + 0);
        shift_longs<14, 18, MASKS32[7]>(src, dest + 14);
        shift_longs<14, 11, MASKS32[7]>(src, dest + 28);
        shift_longs<14, 4, MASKS32[7]>(src, dest + 42);
        return 56;
      case 8U:in.skip_bytes(8 * 16);
        shift_longs<16, 24, MASKS32[8]>(src, dest + 0);
        shift_longs<16, 16, MASKS32[8]>(src, dest + 16);
        shift_longs<16, 8, MASKS32[8]>(src, dest + 32);
        shift_longs<16, 0, MASKS32[8]>(src, dest + 48);
        return 64;
      case 9U:in.skip_bytes(8 * 18);
        shift_longs<18, 23, MASKS32[9]>(src, dest + 0);
        shift_longs<18, 14, MASKS32[9]>(src, dest + 18);
        shift_longs<18, 5, MASKS32[9]>(src, dest + 36);
        return 54;
      case 10U:in.skip_bytes(8 * 20);
        shift_longs<20, 22, MASKS32[10]>(src, dest + 0);
        shift_longs<20, 12, MASKS32[10]>(src, dest + 20);
        shift_longs<20, 2, MASKS32[10]>(src, dest + 40);
        return 60;
      case 11U:in.skip_bytes(8 * 22);
        shift_longs<22, 21, MASKS32[11]>(src, dest + 0);
        shift_longs<22, 10, MASKS32[11]>(src, dest + 22);
        return 44;
      case 12U:in.skip_bytes(8 * 24);
        shift_longs<24, 20, MASKS32[12]>(src, dest + 0);
        shift_longs<24, 8, MASKS32[12]>(src, dest + 24);
        return 48;
      case 13U:in.skip_bytes(8 * 26);
        shift_longs<26, 19, MASKS32[13]>(src, dest + 0);
        shift_longs<26, 6, MASKS32[13]>(src, dest + 26);
        return 52;
      case 14U:in.skip_bytes(8 * 28);
        shift_longs<28, 18, MASKS32[14]>(src, dest + 0);
        shift_longs<28, 4, MASKS32[14]>(src, dest + 28);
        return 56;
      case 15U:in.skip_bytes(8 * 30);
        shift_longs<30, 17, MASKS32[15]>(src, dest + 0);
        shift_longs<30, 2, MASKS32[15]>(src, dest + 30);
        return 60;
      case 16U:in.skip_bytes(8 * 32);
        shift_longs<32, 16, MASKS32[16]>(src, dest + 0);
        shift_longs<32, 0, MASKS32[16]>(src, dest + 32);
        return 64;
      case 17U:in.skip_bytes(8 * 34);
        shift_longs<34, 15, MASKS32[17]>(src, dest + 0);
        return 34;
      case 18U:in.skip_bytes(8 * 36);
        shift_longs<36, 14, MASKS32[18]>(src, dest + 0);
        return 36;
      case 19U:in.skip_bytes(8 * 38);
        shift_longs<38, 13, MASKS32[19]>(src, dest + 0);
        return 38;
      case 20U:in.skip_bytes(8 * 40);
        shift_longs<40, 12, MASKS32[20]>(src, dest + 0);
        return 40;
      case 21U:in.skip_bytes(8 * 42);
        shift_longs<42, 11, MASKS32[21]>(src, dest + 0);
        return 42;
      case 22U:in.skip_bytes(8 * 44);
        shift_longs<44, 10, MASKS32[22]>(src, dest + 0);
        return 44;
      case 23U:in.skip_bytes(8 * 46);
        shift_longs<46, 9, MASKS32[23]>(src, dest + 0);
        return 46;
      case 24U:in.skip_bytes(8 * 48);
        shift_longs<48, 8, MASKS32[24]>(src, dest + 0);
        return 48;
      case 25U:in.skip_bytes(8 * 50);
        shift_longs<50, 7, MASKS32[25]>(src, dest + 0);
        return 50;
      case 26U:in.skip_bytes(8 * 52);
        shift_longs<52, 6, MASKS32[26]>(src, dest + 0);
        return 52;
      case 27U:in.skip_bytes(8 * 54);
        shift_longs<54, 5, MASKS32[27]>(src, dest + 0);
        return 54;
      case 28U:in.skip_bytes(8 * 56);
        shift_longs<56, 4, MASKS32[28]>(src, dest + 0);
        return 56;
      case 29U:in.skip_bytes(8 * 58);
        shift_longs<58, 3, MASKS32[29]>(src, dest + 0);
        return 58;
      case 30U:in.skip_bytes(8 * 60);
        shift_longs<60, 2, MASKS32[30]>(src, dest + 0);
        return 60;
      case 31U:in.skip_bytes(8 * 62);
        shift_longs<62, 1, MASKS32[31]>(src, dest + 0);
        return 62;
      case 32U:shift_longs<64, 0, ~0LLU>(src, dest);
        return 64;
      default:
        // TODO : this is marked as no exception thrown
        throw 13;
    }
  }

  // mark values in array as final longs to avoid the get_cost of reading array, arrays should only be
  // used when the idx is a variable
  static constexpr uint64_t MASK8_1 = MASKS8[1];
  static constexpr uint64_t MASK8_2 = MASKS8[2];
  static constexpr uint64_t MASK8_3 = MASKS8[3];
  static constexpr uint64_t MASK8_4 = MASKS8[4];
  static constexpr uint64_t MASK8_5 = MASKS8[5];
  static constexpr uint64_t MASK8_6 = MASKS8[6];
  static constexpr uint64_t MASK8_7 = MASKS8[7];
  static constexpr uint64_t MASK16_1 = MASKS16[1];
  static constexpr uint64_t MASK16_2 = MASKS16[2];
  static constexpr uint64_t MASK16_3 = MASKS16[3];
  static constexpr uint64_t MASK16_4 = MASKS16[4];
  static constexpr uint64_t MASK16_5 = MASKS16[5];
  static constexpr uint64_t MASK16_6 = MASKS16[6];
  static constexpr uint64_t MASK16_7 = MASKS16[7];
  static constexpr uint64_t MASK16_9 = MASKS16[9];
  static constexpr uint64_t MASK16_10 = MASKS16[10];
  static constexpr uint64_t MASK16_11 = MASKS16[11];
  static constexpr uint64_t MASK16_12 = MASKS16[12];
  static constexpr uint64_t MASK16_13 = MASKS16[13];
  static constexpr uint64_t MASK16_14 = MASKS16[14];
  static constexpr uint64_t MASK16_15 = MASKS16[15];
  static constexpr uint64_t MASK32_1 = MASKS32[1];
  static constexpr uint64_t MASK32_2 = MASKS32[2];
  static constexpr uint64_t MASK32_3 = MASKS32[3];
  static constexpr uint64_t MASK32_4 = MASKS32[4];
  static constexpr uint64_t MASK32_5 = MASKS32[5];
  static constexpr uint64_t MASK32_6 = MASKS32[6];
  static constexpr uint64_t MASK32_7 = MASKS32[7];
  static constexpr uint64_t MASK32_8 = MASKS32[8];
  static constexpr uint64_t MASK32_9 = MASKS32[9];
  static constexpr uint64_t MASK32_10 = MASKS32[10];
  static constexpr uint64_t MASK32_11 = MASKS32[11];
  static constexpr uint64_t MASK32_12 = MASKS32[12];
  static constexpr uint64_t MASK32_13 = MASKS32[13];
  static constexpr uint64_t MASK32_14 = MASKS32[14];
  static constexpr uint64_t MASK32_15 = MASKS32[15];
  static constexpr uint64_t MASK32_17 = MASKS32[17];
  static constexpr uint64_t MASK32_18 = MASKS32[18];
  static constexpr uint64_t MASK32_19 = MASKS32[19];
  static constexpr uint64_t MASK32_20 = MASKS32[20];
  static constexpr uint64_t MASK32_21 = MASKS32[21];
  static constexpr uint64_t MASK32_22 = MASKS32[22];
  static constexpr uint64_t MASK32_23 = MASKS32[23];
  static constexpr uint64_t MASK32_24 = MASKS32[24];

  /** Decode 128 integers into {@code longs}. */
  static void decode(const uint32_t bits_per_value,
                     store::DataInput<store::ReadOnlyDataInput> &in,
                     int64_t *longs) noexcept {
    switch (bits_per_value) {
      case 1U:decode1(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 2U:decode2(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 3U:decode3(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 4U:decode4(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 5U:decode5(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 6U:decode6(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 7U:decode7(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 8U:decode8(in, (uint64_t *) longs);
        expand8((uint64_t *) longs);
        break;
      case 9U:decode9(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 10U:decode10(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 11U:decode11(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 12U:decode12(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 13U:decode13(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 14U:decode14(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 15U:decode15(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 16U:decode16(in, (uint64_t *) longs);
        expand16((uint64_t *) longs);
        break;
      case 17U:decode17(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 18U:decode18(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 19U:decode19(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 20U:decode20(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 21U:decode21(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 22U:decode22(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 23U:decode23(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      case 24U:decode24(in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
      default:decode_slow(bits_per_value, in, (uint64_t *) longs);
        expand32((uint64_t *) longs);
        break;
    }
  }

  /**
   * Decodes 128 integers into 64 {@code longs} such that each long contains two values, each
   * represented with 32 bits. Values [0..63] are encoded in the high-order bits of {@code longs}
   * [0..63], and values [64..127] are encoded in the low-order bits of {@code longs} [0..63]. This
   * representation may allow subsequent operations to be performed on two values at a time.
   */
  static void decode_to32(const uint32_t bits_per_value,
                          store::DataInput<store::ReadOnlyDataInput> &in,
                          int64_t *longs) {
    switch (bits_per_value) {
      case 1:decode1(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 2:decode2(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 3:decode3(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 4:decode4(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 5:decode5(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 6:decode6(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 7:decode7(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 8:decode8(in, (uint64_t *) longs);
        expand8_to32((uint64_t *) longs);
        break;
      case 9:decode9(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 10:decode10(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 11:decode11(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 12:decode12(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 13:decode13(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 14:decode14(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 15:decode15(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 16:decode16(in, (uint64_t *) longs);
        expand16_to32((uint64_t *) longs);
        break;
      case 17:decode17(in, (uint64_t *) longs);
        break;
      case 18:decode18(in, (uint64_t *) longs);
        break;
      case 19:decode19(in, (uint64_t *) longs);
        break;
      case 20:decode20(in, (uint64_t *) longs);
        break;
      case 21:decode21(in, (uint64_t *) longs);
        break;
      case 22:decode22(in, (uint64_t *) longs);
        break;
      case 23:decode23(in, (uint64_t *) longs);
        break;
      case 24:decode24(in, (uint64_t *) longs);
        break;
      default:decode_slow(bits_per_value, in, (uint64_t *) longs);
        break;
    }
  }

  static void decode1(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 2);
    shift_longs<2, 7, MASK8_1>(tmp, longs + 0);
    shift_longs<2, 6, MASK8_1>(tmp, longs + 2);
    shift_longs<2, 5, MASK8_1>(tmp, longs + 4);
    shift_longs<2, 4, MASK8_1>(tmp, longs + 6);
    shift_longs<2, 3, MASK8_1>(tmp, longs + 8);
    shift_longs<2, 2, MASK8_1>(tmp, longs + 10);
    shift_longs<2, 1, MASK8_1>(tmp, longs + 12);
    shift_longs<2, 0, MASK8_1>(tmp, longs + 14);
  }

  static void decode2(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 4);
    shift_longs<4, 6, MASK8_2>(tmp, longs + 0);
    shift_longs<4, 4, MASK8_2>(tmp, longs + 4);
    shift_longs<4, 2, MASK8_2>(tmp, longs + 8);
    shift_longs<4, 0, MASK8_2>(tmp, longs + 12);
  }

  static void decode3(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 6);
    shift_longs<6, 5, MASK8_3>(tmp, longs + 0);
    shift_longs<6, 2, MASK8_3>(tmp, longs + 6);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 12; iter < 2; ++iter, tmp_idx += 3, longs_idx += 2) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK8_2) << 1U;
      l0 |= (tmp[tmp_idx + 1] >> 1U) & MASK8_1;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK8_1) << 2U;
      l1 |= (tmp[tmp_idx + 2] & MASK8_2) << 0U;
      longs[longs_idx + 1] = l1;
    }  // End for
  }

  static void decode4(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 8);
    shift_longs<8, 4, MASK8_4>(tmp, longs + 0);
    shift_longs<8, 0, MASK8_4>(tmp, longs + 8);
  }

  static void decode5(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 10);
    shift_longs<10, 3, MASK8_5>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 10; iter < 2; ++iter, tmp_idx += 5, longs_idx += 3) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK8_3) << 2U;
      l0 |= (tmp[tmp_idx + 1] >> 1U) & MASK8_2;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK8_1) << 4U;
      l1 |= (tmp[tmp_idx + 2] & MASK8_3) << 1U;
      l1 |= (tmp[tmp_idx + 3] >> 2U) & MASK8_1;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 3] & MASK8_2) << 3U;
      l2 |= (tmp[tmp_idx + 4] & MASK8_3) << 0U;
      longs[longs_idx + 2] = l2;
    }  // End for
  }

  static void decode6(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 12);
    shift_longs<12, 2, MASK8_6>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 12; iter < 4; ++iter, tmp_idx += 3, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK8_2) << 4U;
      l0 |= (tmp[tmp_idx + 1] & MASK8_2) << 2U;
      l0 |= (tmp[tmp_idx + 2] & MASK8_2) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }

  static void decode7(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 14);
    shift_longs<14, 1, MASK8_7>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 14; iter < 2; ++iter, tmp_idx += 7, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK8_1) << 6U;
      l0 |= (tmp[tmp_idx + 1] & MASK8_1) << 5U;
      l0 |= (tmp[tmp_idx + 2] & MASK8_1) << 4U;
      l0 |= (tmp[tmp_idx + 3] & MASK8_1) << 3U;
      l0 |= (tmp[tmp_idx + 4] & MASK8_1) << 2U;
      l0 |= (tmp[tmp_idx + 5] & MASK8_1) << 1U;
      l0 |= (tmp[tmp_idx + 6] & MASK8_1) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }

  static void decode8(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    in.read_longs<16>(longs);
  }

  static void decode9(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 18);
    shift_longs<18, 7, MASK16_9>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 18; iter < 2; ++iter, tmp_idx += 9, longs_idx += 7) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_7) << 2U;
      l0 |= (tmp[tmp_idx + 1] >> 5U) & MASK16_2;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK16_5) << 4U;
      l1 |= (tmp[tmp_idx + 2] >> 3U) & MASK16_4;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 2] & MASK16_3) << 6U;
      l2 |= (tmp[tmp_idx + 3] >> 1U) & MASK16_6;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 3] & MASK16_1) << 8U;
      l3 |= (tmp[tmp_idx + 4] & MASK16_7) << 1U;
      l3 |= (tmp[tmp_idx + 5] >> 6U) & MASK16_1;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 5] & MASK16_6) << 3U;
      l4 |= (tmp[tmp_idx + 6] >> 4U) & MASK16_3;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 6] & MASK16_4) << 5U;
      l5 |= (tmp[tmp_idx + 7] >> 2U) & MASK16_5;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 7] & MASK16_2) << 7U;
      l6 |= (tmp[tmp_idx + 8] & MASK16_7) << 0U;
      longs[longs_idx + 6] = l6;
    }  // End for
  }

  static void decode10(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 20);
    shift_longs<20, 6, MASK16_10>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 20; iter < 4; ++iter, tmp_idx += 5, longs_idx += 3) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_6) << 4U;
      l0 |= (tmp[tmp_idx + 1] >> 2U) & MASK16_4;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK16_2) << 8U;
      l1 |= (tmp[tmp_idx + 2] & MASK16_6) << 2U;
      l1 |= (tmp[tmp_idx + 3] >> 4U) & MASK16_2;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 3] & MASK16_4) << 6U;
      l2 |= (tmp[tmp_idx + 4] & MASK16_6) << 0U;
      longs[longs_idx + 2] = l2;
    }  // End for
  }

  static void decode11(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 22);
    shift_longs<22, 5, MASK16_11>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 22; iter < 2; ++iter, tmp_idx += 11, longs_idx += 5) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_5) << 6U;
      l0 |= (tmp[tmp_idx + 1] & MASK16_5) << 1U;
      l0 |= (tmp[tmp_idx + 2] >> 4U) & MASK16_1;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 2] & MASK16_4) << 7U;
      l1 |= (tmp[tmp_idx + 3] & MASK16_5) << 2U;
      l1 |= (tmp[tmp_idx + 4] >> 3U) & MASK16_2;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 4] & MASK16_3) << 8U;
      l2 |= (tmp[tmp_idx + 5] & MASK16_5) << 3U;
      l2 |= (tmp[tmp_idx + 6] >> 2U) & MASK16_3;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 6] & MASK16_2) << 9U;
      l3 |= (tmp[tmp_idx + 7] & MASK16_5) << 4U;
      l3 |= (tmp[tmp_idx + 8] >> 1U) & MASK16_4;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 8] & MASK16_1) << 10U;
      l4 |= (tmp[tmp_idx + 9] & MASK16_5) << 5U;
      l4 |= (tmp[tmp_idx + 10] & MASK16_5) << 0U;
      longs[longs_idx + 4] = l4;
    }  // End for
  }

  static void decode12(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 24);
    shift_longs<24, 4, MASK16_12>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 24; iter < 8; ++iter, tmp_idx += 3, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_4) << 8U;
      l0 |= (tmp[tmp_idx + 1] & MASK16_4) << 4U;
      l0 |= (tmp[tmp_idx + 2] & MASK16_4) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }

  static void decode13(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 26);
    shift_longs<26, 3, MASK16_13>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 26; iter < 2; ++iter, tmp_idx += 13, longs_idx += 3) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_3) << 10U;
      l0 |= (tmp[tmp_idx + 1] & MASK16_3) << 7U;
      l0 |= (tmp[tmp_idx + 2] & MASK16_3) << 4U;
      l0 |= (tmp[tmp_idx + 3] & MASK16_3) << 1U;
      l0 |= (tmp[tmp_idx + 4] >> 2U) & MASK16_1;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 4] & MASK16_2) << 11U;
      l1 |= (tmp[tmp_idx + 5] & MASK16_3) << 8U;
      l1 |= (tmp[tmp_idx + 6] & MASK16_3) << 5U;
      l1 |= (tmp[tmp_idx + 7] & MASK16_3) << 2U;
      l1 |= (tmp[tmp_idx + 8] >> 1U) & MASK16_2;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 8] & MASK16_1) << 12U;
      l2 |= (tmp[tmp_idx + 9] & MASK16_3) << 9U;
      l2 |= (tmp[tmp_idx + 10] & MASK16_3) << 6U;
      l2 |= (tmp[tmp_idx + 11] & MASK16_3) << 3U;
      l2 |= (tmp[tmp_idx + 12] & MASK16_3) << 0U;
      longs[longs_idx + 2] = l2;
    }  // End for
  }

  static void decode14(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 28);
    shift_longs<28, 2, MASK16_14>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 28; iter < 4; ++iter, tmp_idx += 7, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_2) << 12U;
      l0 |= (tmp[tmp_idx + 1] & MASK16_2) << 10U;
      l0 |= (tmp[tmp_idx + 2] & MASK16_2) << 8U;
      l0 |= (tmp[tmp_idx + 3] & MASK16_2) << 6U;
      l0 |= (tmp[tmp_idx + 4] & MASK16_2) << 4U;
      l0 |= (tmp[tmp_idx + 5] & MASK16_2) << 2U;
      l0 |= (tmp[tmp_idx + 6] & MASK16_2) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }

  static void decode15(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 30);
    shift_longs<30, 1, MASK16_15>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 30; iter < 2; ++iter, tmp_idx += 15, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK16_1) << 14U;
      l0 |= (tmp[tmp_idx + 1] & MASK16_1) << 13U;
      l0 |= (tmp[tmp_idx + 2] & MASK16_1) << 12U;
      l0 |= (tmp[tmp_idx + 3] & MASK16_1) << 11U;
      l0 |= (tmp[tmp_idx + 4] & MASK16_1) << 10U;
      l0 |= (tmp[tmp_idx + 5] & MASK16_1) << 9U;
      l0 |= (tmp[tmp_idx + 6] & MASK16_1) << 8U;
      l0 |= (tmp[tmp_idx + 7] & MASK16_1) << 7U;
      l0 |= (tmp[tmp_idx + 8] & MASK16_1) << 6U;
      l0 |= (tmp[tmp_idx + 9] & MASK16_1) << 5U;
      l0 |= (tmp[tmp_idx + 10] & MASK16_1) << 4U;
      l0 |= (tmp[tmp_idx + 11] & MASK16_1) << 3U;
      l0 |= (tmp[tmp_idx + 12] & MASK16_1) << 2U;
      l0 |= (tmp[tmp_idx + 13] & MASK16_1) << 1U;
      l0 |= (tmp[tmp_idx + 14] & MASK16_1) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }

  static void decode16(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    in.read_longs<32>(longs);
  }

  static void decode17(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 34);
    shift_longs<34, 15, MASK32_17>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 34; iter < 2; ++iter, tmp_idx += 17, longs_idx += 15) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_15) << 2U;
      l0 |= (tmp[tmp_idx + 1] >> 13U) & MASK32_2;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK32_13) << 4U;
      l1 |= (tmp[tmp_idx + 2] >> 11U) & MASK32_4;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 2] & MASK32_11) << 6U;
      l2 |= (tmp[tmp_idx + 3] >> 9U) & MASK32_6;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 3] & MASK32_9) << 8U;
      l3 |= (tmp[tmp_idx + 4] >> 7U) & MASK32_8;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 4] & MASK32_7) << 10U;
      l4 |= (tmp[tmp_idx + 5] >> 5U) & MASK32_10;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 5] & MASK32_5) << 12U;
      l5 |= (tmp[tmp_idx + 6] >> 3U) & MASK32_12;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 6] & MASK32_3) << 14U;
      l6 |= (tmp[tmp_idx + 7] >> 1U) & MASK32_14;
      longs[longs_idx + 6] = l6;
      uint64_t l7 = (tmp[tmp_idx + 7] & MASK32_1) << 16U;
      l7 |= (tmp[tmp_idx + 8] & MASK32_15) << 1U;
      l7 |= (tmp[tmp_idx + 9] >> 14U) & MASK32_1;
      longs[longs_idx + 7] = l7;
      uint64_t l8 = (tmp[tmp_idx + 9] & MASK32_14) << 3U;
      l8 |= (tmp[tmp_idx + 10] >> 12U) & MASK32_3;
      longs[longs_idx + 8] = l8;
      uint64_t l9 = (tmp[tmp_idx + 10] & MASK32_12) << 5U;
      l9 |= (tmp[tmp_idx + 11] >> 10U) & MASK32_5;
      longs[longs_idx + 9] = l9;
      uint64_t l10 = (tmp[tmp_idx + 11] & MASK32_10) << 7U;
      l10 |= (tmp[tmp_idx + 12] >> 8U) & MASK32_7;
      longs[longs_idx + 10] = l10;
      uint64_t l11 = (tmp[tmp_idx + 12] & MASK32_8) << 9U;
      l11 |= (tmp[tmp_idx + 13] >> 6U) & MASK32_9;
      longs[longs_idx + 11] = l11;
      uint64_t l12 = (tmp[tmp_idx + 13] & MASK32_6) << 11U;
      l12 |= (tmp[tmp_idx + 14] >> 4U) & MASK32_11;
      longs[longs_idx + 12] = l12;
      uint64_t l13 = (tmp[tmp_idx + 14] & MASK32_4) << 13U;
      l13 |= (tmp[tmp_idx + 15] >> 2U) & MASK32_13;
      longs[longs_idx + 13] = l13;
      uint64_t l14 = (tmp[tmp_idx + 15] & MASK32_2) << 15U;
      l14 |= (tmp[tmp_idx + 16] & MASK32_15) << 0U;
      longs[longs_idx + 14] = l14;
    }  // End for
  }

  static void decode18(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 36);
    shift_longs<36, 14, MASK32_18>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 36; iter < 4; ++iter, tmp_idx += 9, longs_idx += 7) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_14) << 4U;
      l0 |= (tmp[tmp_idx + 1] >> 10U) & MASK32_4;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK32_10) << 8U;
      l1 |= (tmp[tmp_idx + 2] >> 6U) & MASK32_8;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 2] & MASK32_6) << 12U;
      l2 |= (tmp[tmp_idx + 3] >> 2U) & MASK32_12;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 3] & MASK32_2) << 16U;
      l3 |= (tmp[tmp_idx + 4] & MASK32_14) << 2U;
      l3 |= (tmp[tmp_idx + 5] >> 12U) & MASK32_2;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 5] & MASK32_12) << 6U;
      l4 |= (tmp[tmp_idx + 6] >> 8U) & MASK32_6;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 6] & MASK32_8) << 10U;
      l5 |= (tmp[tmp_idx + 7] >> 4U) & MASK32_10;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 7] & MASK32_4) << 14U;
      l6 |= (tmp[tmp_idx + 8] & MASK32_14) << 0U;
      longs[longs_idx + 6] = l6;
    }  // End for
  }

  static void decode19(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 38);
    shift_longs<38, 13, MASK32_19>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 38; iter < 2; ++iter, tmp_idx += 19, longs_idx += 13) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_13) << 6U;
      l0 |= (tmp[tmp_idx + 1] >> 7U) & MASK32_6;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK32_7) << 12U;
      l1 |= (tmp[tmp_idx + 2] >> 1U) & MASK32_12;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 2] & MASK32_1) << 18U;
      l2 |= (tmp[tmp_idx + 3] & MASK32_13) << 5U;
      l2 |= (tmp[tmp_idx + 4] >> 8U) & MASK32_5;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 4] & MASK32_8) << 11U;
      l3 |= (tmp[tmp_idx + 5] >> 2U) & MASK32_11;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 5] & MASK32_2) << 17U;
      l4 |= (tmp[tmp_idx + 6] & MASK32_13) << 4U;
      l4 |= (tmp[tmp_idx + 7] >> 9U) & MASK32_4;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 7] & MASK32_9) << 10U;
      l5 |= (tmp[tmp_idx + 8] >> 3U) & MASK32_10;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 8] & MASK32_3) << 16U;
      l6 |= (tmp[tmp_idx + 9] & MASK32_13) << 3U;
      l6 |= (tmp[tmp_idx + 10] >> 10U) & MASK32_3;
      longs[longs_idx + 6] = l6;
      uint64_t l7 = (tmp[tmp_idx + 10] & MASK32_10) << 9U;
      l7 |= (tmp[tmp_idx + 11] >> 4U) & MASK32_9;
      longs[longs_idx + 7] = l7;
      uint64_t l8 = (tmp[tmp_idx + 11] & MASK32_4) << 15U;
      l8 |= (tmp[tmp_idx + 12] & MASK32_13) << 2U;
      l8 |= (tmp[tmp_idx + 13] >> 11U) & MASK32_2;
      longs[longs_idx + 8] = l8;
      uint64_t l9 = (tmp[tmp_idx + 13] & MASK32_11) << 8U;
      l9 |= (tmp[tmp_idx + 14] >> 5U) & MASK32_8;
      longs[longs_idx + 9] = l9;
      uint64_t l10 = (tmp[tmp_idx + 14] & MASK32_5) << 14U;
      l10 |= (tmp[tmp_idx + 15] & MASK32_13) << 1U;
      l10 |= (tmp[tmp_idx + 16] >> 12U) & MASK32_1;
      longs[longs_idx + 10] = l10;
      uint64_t l11 = (tmp[tmp_idx + 16] & MASK32_12) << 7U;
      l11 |= (tmp[tmp_idx + 17] >> 6U) & MASK32_7;
      longs[longs_idx + 11] = l11;
      uint64_t l12 = (tmp[tmp_idx + 17] & MASK32_6) << 13U;
      l12 |= (tmp[tmp_idx + 18] & MASK32_13) << 0U;
      longs[longs_idx + 12] = l12;
    }  // End for
  }

  static void decode20(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 40);
    shift_longs<40, 12, MASK32_20>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 40; iter < 8; ++iter, tmp_idx += 5, longs_idx += 3) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_12) << 8U;
      l0 |= (tmp[tmp_idx + 1] >> 4U) & MASK32_8;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK32_4) << 16U;
      l1 |= (tmp[tmp_idx + 2] & MASK32_12) << 4U;
      l1 |= (tmp[tmp_idx + 3] >> 8U) & MASK32_4;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 3] & MASK32_8) << 12U;
      l2 |= (tmp[tmp_idx + 4] & MASK32_12) << 0U;
      longs[longs_idx + 2] = l2;
    }  // End for
  }

  static void decode21(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 42);
    shift_longs<42, 11, MASK32_21>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 42; iter < 2; ++iter, tmp_idx += 21, longs_idx += 11) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_11) << 10U;
      l0 |= (tmp[tmp_idx + 1] >> 1U) & MASK32_10;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 1] & MASK32_1) << 20U;
      l1 |= (tmp[tmp_idx + 2] & MASK32_11) << 9U;
      l1 |= (tmp[tmp_idx + 3] >> 2U) & MASK32_9;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 3] & MASK32_2) << 19U;
      l2 |= (tmp[tmp_idx + 4] & MASK32_11) << 8U;
      l2 |= (tmp[tmp_idx + 5] >> 3U) & MASK32_8;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 5] & MASK32_3) << 18U;
      l3 |= (tmp[tmp_idx + 6] & MASK32_11) << 7U;
      l3 |= (tmp[tmp_idx + 7] >> 4U) & MASK32_7;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 7] & MASK32_4) << 17U;
      l4 |= (tmp[tmp_idx + 8] & MASK32_11) << 6U;
      l4 |= (tmp[tmp_idx + 9] >> 5U) & MASK32_6;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 9] & MASK32_5) << 16U;
      l5 |= (tmp[tmp_idx + 10] & MASK32_11) << 5U;
      l5 |= (tmp[tmp_idx + 11] >> 6U) & MASK32_5;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 11] & MASK32_6) << 15U;
      l6 |= (tmp[tmp_idx + 12] & MASK32_11) << 4U;
      l6 |= (tmp[tmp_idx + 13] >> 7U) & MASK32_4;
      longs[longs_idx + 6] = l6;
      uint64_t l7 = (tmp[tmp_idx + 13] & MASK32_7) << 14U;
      l7 |= (tmp[tmp_idx + 14] & MASK32_11) << 3U;
      l7 |= (tmp[tmp_idx + 15] >> 8U) & MASK32_3;
      longs[longs_idx + 7] = l7;
      uint64_t l8 = (tmp[tmp_idx + 15] & MASK32_8) << 13U;
      l8 |= (tmp[tmp_idx + 16] & MASK32_11) << 2U;
      l8 |= (tmp[tmp_idx + 17] >> 9U) & MASK32_2;
      longs[longs_idx + 8] = l8;
      uint64_t l9 = (tmp[tmp_idx + 17] & MASK32_9) << 12U;
      l9 |= (tmp[tmp_idx + 18] & MASK32_11) << 1U;
      l9 |= (tmp[tmp_idx + 19] >> 10U) & MASK32_1;
      longs[longs_idx + 9] = l9;
      uint64_t l10 = (tmp[tmp_idx + 19] & MASK32_10) << 11U;
      l10 |= (tmp[tmp_idx + 20] & MASK32_11) << 0U;
      longs[longs_idx + 10] = l10;
    }  // End for
  }

  static void decode22(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 44);
    shift_longs<44, 10, MASK32_22>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 44; iter < 4; ++iter, tmp_idx += 11, longs_idx += 5) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_10) << 12U;
      l0 |= (tmp[tmp_idx + 1] & MASK32_10) << 2U;
      l0 |= (tmp[tmp_idx + 2] >> 8U) & MASK32_2;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 2] & MASK32_8) << 14U;
      l1 |= (tmp[tmp_idx + 3] & MASK32_10) << 4U;
      l1 |= (tmp[tmp_idx + 4] >> 6U) & MASK32_4;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 4] & MASK32_6) << 16U;
      l2 |= (tmp[tmp_idx + 5] & MASK32_10) << 6U;
      l2 |= (tmp[tmp_idx + 6] >> 4U) & MASK32_6;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 6] & MASK32_4) << 18U;
      l3 |= (tmp[tmp_idx + 7] & MASK32_10) << 8U;
      l3 |= (tmp[tmp_idx + 8] >> 2U) & MASK32_8;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 8] & MASK32_2) << 20U;
      l4 |= (tmp[tmp_idx + 9] & MASK32_10) << 10U;
      l4 |= (tmp[tmp_idx + 10] & MASK32_10) << 0U;
      longs[longs_idx + 4] = l4;
    }  // End for
  }

  static void decode23(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 46);
    shift_longs<46, 9, MASK32_23>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 46; iter < 2; ++iter, tmp_idx += 23, longs_idx += 9) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_9) << 14U;
      l0 |= (tmp[tmp_idx + 1] & MASK32_9) << 5U;
      l0 |= (tmp[tmp_idx + 2] >> 4U) & MASK32_5;
      longs[longs_idx + 0] = l0;
      uint64_t l1 = (tmp[tmp_idx + 2] & MASK32_4) << 19U;
      l1 |= (tmp[tmp_idx + 3] & MASK32_9) << 10U;
      l1 |= (tmp[tmp_idx + 4] & MASK32_9) << 1U;
      l1 |= (tmp[tmp_idx + 5] >> 8U) & MASK32_1;
      longs[longs_idx + 1] = l1;
      uint64_t l2 = (tmp[tmp_idx + 5] & MASK32_8) << 15U;
      l2 |= (tmp[tmp_idx + 6] & MASK32_9) << 6U;
      l2 |= (tmp[tmp_idx + 7] >> 3U) & MASK32_6;
      longs[longs_idx + 2] = l2;
      uint64_t l3 = (tmp[tmp_idx + 7] & MASK32_3) << 20U;
      l3 |= (tmp[tmp_idx + 8] & MASK32_9) << 11U;
      l3 |= (tmp[tmp_idx + 9] & MASK32_9) << 2U;
      l3 |= (tmp[tmp_idx + 10] >> 7U) & MASK32_2;
      longs[longs_idx + 3] = l3;
      uint64_t l4 = (tmp[tmp_idx + 10] & MASK32_7) << 16U;
      l4 |= (tmp[tmp_idx + 11] & MASK32_9) << 7U;
      l4 |= (tmp[tmp_idx + 12] >> 2U) & MASK32_7;
      longs[longs_idx + 4] = l4;
      uint64_t l5 = (tmp[tmp_idx + 12] & MASK32_2) << 21U;
      l5 |= (tmp[tmp_idx + 13] & MASK32_9) << 12U;
      l5 |= (tmp[tmp_idx + 14] & MASK32_9) << 3U;
      l5 |= (tmp[tmp_idx + 15] >> 6U) & MASK32_3;
      longs[longs_idx + 5] = l5;
      uint64_t l6 = (tmp[tmp_idx + 15] & MASK32_6) << 17U;
      l6 |= (tmp[tmp_idx + 16] & MASK32_9) << 8U;
      l6 |= (tmp[tmp_idx + 17] >> 1U) & MASK32_8;
      longs[longs_idx + 6] = l6;
      uint64_t l7 = (tmp[tmp_idx + 17] & MASK32_1) << 22U;
      l7 |= (tmp[tmp_idx + 18] & MASK32_9) << 13U;
      l7 |= (tmp[tmp_idx + 19] & MASK32_9) << 4U;
      l7 |= (tmp[tmp_idx + 20] >> 5U) & MASK32_4;
      longs[longs_idx + 7] = l7;
      uint64_t l8 = (tmp[tmp_idx + 20] & MASK32_5) << 18U;
      l8 |= (tmp[tmp_idx + 21] & MASK32_9) << 9U;
      l8 |= (tmp[tmp_idx + 22] & MASK32_9) << 0U;
      longs[longs_idx + 8] = l8;
    }  // End for
  }

  static void decode24(store::DataInput<store::ReadOnlyDataInput> &in, uint64_t *longs) noexcept {
    const auto tmp = (const uint64_t *) in.input.data;
    in.skip_bytes(8 * 48);
    shift_longs<48, 8, MASK32_24>(tmp, longs + 0);
    for (int32_t iter = 0, tmp_idx = 0, longs_idx = 48; iter < 16; ++iter, tmp_idx += 3, longs_idx += 1) {
      uint64_t l0 = (tmp[tmp_idx + 0] & MASK32_8) << 16U;
      l0 |= (tmp[tmp_idx + 1] & MASK32_8) << 8U;
      l0 |= (tmp[tmp_idx + 2] & MASK32_8) << 0U;
      longs[longs_idx + 0] = l0;
    }
  }
};  // ForUtil

}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_FORUTIL_HPP_