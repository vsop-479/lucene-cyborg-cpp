#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_FST_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_FST_HPP_

#include <bit>
#include <cstdint>
#include <algorithm>
#include <optional>
#include "../store/DataInput.hpp"
#include "../store/CloseableMemoryDataInput.hpp"
#include "../codecs/CodecUtil.hpp"
#include "FstReverseBytesReader.hpp"

namespace lucene::cyborg::util {

// Requirements of the type T
//   - bool operator==(const T&) const;
//   - bool operator!=(const T&) const;

// Expected methods fst_in Output
//   - const T& get_no_output();
//   - void skip_output(FstReverseBytesReader&);
//   - void skip_final_output(FstReverseBytesReader&);
//   - void read_output(FstReverseBytesReader&, T&);
//   - void read_final_output(FstReverseBytesReader&, T&);

inline const std::string FST_FILE_FORMAT_NAME = "FST";
constexpr int32_t FST_END_LABEL = -1;
constexpr int64_t FST_FINAL_END_NODE = -1;
constexpr int32_t FST_VERSION_LITTLE_ENDIAN = 8;
constexpr int32_t FST_VERSION_CURRENT = FST_VERSION_LITTLE_ENDIAN;
constexpr int32_t FST_VERSION_START = 6;
constexpr int64_t FINAL_END_NODE = -1;
constexpr int64_t FST_NON_FINAL_END_NODE = -1;
constexpr uint32_t FST_BIT_ARC_HAS_OUTPUT = 1U << 4U;
constexpr uint32_t FST_BIT_STOP_NODE = 1U << 3U;
constexpr uint32_t FST_BIT_TARGET_NEXT = 1U << 2U;
constexpr uint8_t FST_BIT_ARC_HAS_FINAL_OUTPUT = 1U << 5U;
constexpr uint8_t FST_ARCS_FOR_BINARY_SEARCH = FST_BIT_ARC_HAS_FINAL_OUTPUT;
constexpr uint8_t FST_ARCS_FOR_DIRECT_ADDRESSING = 1U << 6U;
constexpr uint8_t FST_BIT_FINAL_ARC = 1U;
constexpr uint8_t FST_BIT_LAST_ARC = 2U;

static void skip_meta_input_in_fst_during_loading(store::DataInput<store::ReadOnlyDataInput> &meta_in) {
  // codec header
  codecs::CodecUtil::check_header(meta_in, FST_FILE_FORMAT_NAME, FST_VERSION_START, FST_VERSION_CURRENT);
  if (meta_in.read_byte() == 1) {
    // accepts empty string
    const int32_t num_bytes = meta_in.read_vint();
    meta_in.skip_bytes(num_bytes);
  }

  // input type
  meta_in.read_byte();
  // start node
  meta_in.read_vlong();
  // index num bytes
  meta_in.read_vlong();
}

static int32_t fst_get_num_presence_bytes(int32_t label_range) noexcept {
  assert(label_range >= 0);
  return (label_range + 7) >> 3;
}

template<typename T>
struct FstArc {
  int64_t target;
  int64_t next_arc;
  int64_t pos_arcs_start;
  int64_t bit_table_start;
  int32_t label;
  int32_t bytes_per_arc;
  int32_t arc_idx;
  int32_t num_arcs;
  int32_t first_label;
  int32_t presence_index;
  T output;
  T next_final_output;
  uint8_t flags;
  int8_t node_flags;

  [[nodiscard]] bool flag(uint8_t flag) const noexcept {
    return (flags & flag);
  }

  [[nodiscard]] bool is_last() const noexcept {
    return (flags & FST_BIT_LAST_ARC);
  }

  [[nodiscard]] bool is_final() const noexcept {
    return (flags & FST_BIT_FINAL_ARC);
  }
};  // FstArc



struct FstBitTable {
  template<typename T>
  static bool is_bit_set(int32_t bit_index, FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    in.seek(arc.bit_table_start);
    assert(bit_index >= 0);
    in.skip_bytes(bit_index >> 3);
    const uint32_t last_byte = in.read_byte();
    return last_byte & (1U << (bit_index & 7U));
  }

  template<typename T>
  static int32_t count_bits(FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    in.seek(arc.bit_table_start);
    const int32_t bit_table_bytes = fst_get_num_presence_bytes(arc.num_arcs);
    assert(bit_table_bytes >= 0);
    int32_t bit_count = 0;
    for (int32_t i = bit_table_bytes / 8; i > 0; --i) {
      // Count the bits set for all plain longs.
      // bit_count += Long.bit_count(in.read_long());
      bit_count += std::popcount<uint64_t>(in.read_long());
    }

    if (int32_t num_remaining_bytes = bit_table_bytes & 63U; num_remaining_bytes != 0) {
      assert(num_remaining_bytes > 0 && num_remaining_bytes <= 8);
      int64_t l = in.read_byte();
      int32_t shift = 0;
      while (--num_remaining_bytes != 0) {
        shift += 8;
        l |= ((uint64_t) in.read_byte()) << shift;
      }
      // bit_count += Long::bit_count(l);
      bit_count += std::popcount<uint64_t>(l);
    }  // End if

    return bit_count;
  }

  template<typename T>
  static int32_t count_bits_up_to(int32_t bit_index, FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    in.seek(arc.bit_table_start);

    assert(bit_index >= 0);
    int32_t bit_count = 0;
    for (int32_t i = bit_index / 64; i > 0; --i) {
      // Count the bits set for all plain longs.
      bit_count += std::popcount<uint64_t>(in.read_long());
    }

    if (int32_t remaining_bits = bit_index % 64U; remaining_bits != 0) {
      int32_t num_remaining_bytes = (remaining_bits + 7) / 8;
      // Count the bits set only within the mask part, so up to bitIndex exclusive.
      assert(num_remaining_bytes > 0 && num_remaining_bytes <= 8);
      uint64_t l = in.read_byte();
      int32_t shift = 0;
      while (--num_remaining_bytes != 0) {
        shift += 8;
        l |= (((uint64_t) in.read_byte()) << shift);
      }

      // Prepare a mask with 1s on the right up to bitIndex exclusive.
      const uint64_t mask = (1ULL << bit_index) - 1L; // Shifts are mod 64.
      bit_count += std::popcount<uint64_t>(l & mask);
    }  // End if

    return bit_count;
  }

  template<typename T>
  static int32_t next_bit_set(int32_t bit_index, FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    in.seek(arc.bit_table_start);
    const int32_t bit_table_bytes = fst_get_num_presence_bytes(arc.num_arcs);
    assert(bit_index >= -1 && bit_index < bit_table_bytes * 8);
    int32_t byte_index = bit_index / 8;
    const uint32_t mask = (~0U) << (((uint32_t) (bit_index + 1)) & 7);
    int32_t i;
    if (bit_index != -1 && mask == (~0U)) {
      // Ex: bit index 23, then byte_index = 2, the next bit will present in the next byte. e.g. after 2 + 1 bytes.
      in.skip_bytes(byte_index + 1);
      i = 0;
    } else {
      in.skip_bytes(byte_index);
      i = in.read_byte() & mask;
    }
    while (i == 0) {
      if (++byte_index == bit_table_bytes) {
        return -1;
      }
      i = in.read_byte();
    }
    return std::countr_zero<uint32_t>(i) + (byte_index << 3);
  }

  template<typename T>
  static int32_t previous_bit_set(int32_t bit_index, FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    in.seek(arc.bit_table_start);

    assert(bit_index >= 0);
    int32_t byte_index = bit_index >> 3;
    in.skip_bytes(byte_index);
    uint32_t mask;
    if (bit_index > 0) {
      mask = (1U << (bit_index & 7U)) - 1;
    } else {
      mask = ~((uint32_t) 0);
    }
    int32_t i = in.read_byte() & mask;
    while (i == 0) {
      if (byte_index-- == 0) {
        return -1;
      }
      in.skip_bytes(-2); // FST.BytesReader implementations support negative skip.
      i = in.read_byte();
    }
    return 31 - std::countl_zero<uint32_t>(i) + (byte_index << 3);
  }

  template<typename T>
  static bool assert_is_valid(FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.bytes_per_arc > 0);
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    // First bit must be set.
    assert(is_bit_set(0, arc, in));
    // Last bit must be set.
    assert(is_bit_set(arc.num_arcs - 1, arc, in));
    // No bit set after the last arc.
    assert(next_bit_set(arc.num_arcs - 1, arc, in) == -1);
    return true;
  }
};  // FstBitTable



template<int32_t InputType, typename T, typename Outputs>
class Fst {
 public:
  Fst(store::DataInput<store::BufferedChecksumMemoryDataInput> &meta_input,
      store::CloseableMemoryDataInput in,
      Outputs outputs)
      : version(),
        empty_output(),
        empty_output_bytes(),
        start_node(),
        num_fst_bytes(),
        fst_in(std::move(in)),
        outputs(std::move(outputs)) {
    // NOTE: only reads formats VERSION_START up to VERSION_CURRENT; we don't have
    // back-compat promise for FSTs (they are experimental), but we are sometimes able to offer it
    version = codecs::CodecUtil::check_header(meta_input, FST_FILE_FORMAT_NAME, FST_VERSION_START, FST_VERSION_CURRENT);
    if (meta_input.read_byte() == 1) {
      // accepts empty string
      const int32_t num_bytes = meta_input.read_vint();
      if (num_bytes > 0) {
        // allocating a buffer
        empty_output_bytes.resize(num_bytes);
        meta_input.read_bytes(empty_output_bytes.data(), num_bytes);

        // deserialize empty string output
        FstReverseBytesReader empty_output_reader{
            (ReadOnlyPtr<uint8_t>) empty_output_bytes.data(), num_bytes
        };

        // NoOutputs uses 0 bytes when writing its output,
        // so we have to check here else BytesStore gets angry fst_in Java.
        empty_output_reader.seek(num_bytes - 1);

        T empty_output_to_move{};
        outputs.read_final_output(empty_output_reader, empty_output_to_move);
        empty_output = std::move(empty_output_to_move);
      }
    }  // End if

    const auto t = meta_input.read_byte();
    if (t == 0) {
      if (InputType != 1) {
        // TODO
        throw 13;
      }
    } else if (t == 1) {
      if (InputType != 2) {
        // TODO
        throw 13;
      }
    } else if (t == 2) {
      if (InputType != 4) {
        // TODO
        throw 13;
      }
    } else {
      // TODO
      throw 13;
    }  // End if

    start_node = meta_input.read_vlong();
    num_fst_bytes = meta_input.read_vlong();
  }

  void get_first_arc(FstArc<T> &arc) {
    const auto &no_output = outputs.get_no_output();

    if (empty_output) {
      arc.flags = FST_BIT_FINAL_ARC | FST_BIT_LAST_ARC;
      arc.next_final_output = *empty_output;
      if (*empty_output != no_output) {
        arc.flags |= FST_BIT_ARC_HAS_FINAL_OUTPUT;
      }
    } else {
      arc.flags = FST_BIT_LAST_ARC;
      arc.next_final_output = no_output;
    }  // End if
    arc.output = no_output;

    // If there are no nodes, i.e., the FST only accepts the empty string,
    // then start_node is 0
    arc.target = start_node;
  }

  bool find_target_arc(int32_t label_to_match, FstArc<T> &follow, FstArc<T> &arc, FstReverseBytesReader &in) {
    if (label_to_match == FST_END_LABEL) {
      if (follow.is_final()) {
        if (follow.target <= 0) {
          arc.flags = FST_BIT_LAST_ARC;
        } else {
          arc.flags = 0;
          // NOTE: next_arc is a node (not an address!) fst_in this case:
          arc.next_arc = follow.target;
        }
        arc.output = follow.next_final_output;
        arc.label = FST_END_LABEL;
        arc.node_flags = arc.flags;
        return true;
      } else {
        return false;
      }
    }  // End if

    if (follow.target <= 0) {
      // follow does not have a target
      return false;
    }

    in.seek(follow.target);

    const auto flags = arc.node_flags = in.read_byte();
    if (flags == FST_ARCS_FOR_DIRECT_ADDRESSING) {
      arc.num_arcs = in.read_vint(); // This is fst_in fact the label range.
      arc.bytes_per_arc = in.read_vint();
      read_presence_bytes(arc, in);
      arc.first_label = read_label(in);
      arc.pos_arcs_start = in.get_file_pointer();

      const int32_t arc_index = label_to_match - arc.first_label;
      if (arc_index < 0 || arc_index >= arc.num_arcs) {
        return false; // Before or after label range.
      } else if (!FstBitTable::is_bit_set(arc_index, arc, in)) {
        return false; // Arc missing fst_in the range.
      }
      read_arc_by_direct_addressing(arc, in, arc_index);
      return true;
    } else if (flags == FST_ARCS_FOR_BINARY_SEARCH) {
      arc.num_arcs = in.read_vint();
      arc.bytes_per_arc = in.read_vint();
      arc.pos_arcs_start = in.get_file_pointer();

      // Array is sparse; do binary search:
      if (arc.num_arcs > 0) {
        uint32_t low = 0;
        uint32_t high = arc.num_arcs - 1;
        while (low <= high) {
          const int32_t mid = (low + high) >> 1U;
          // +1 to skip over flags
          in.seek(arc.pos_arcs_start - (arc.bytes_per_arc * mid + 1));
          const int32_t mid_label = read_label(in);
          const int32_t cmp = mid_label - label_to_match;
          if (cmp < 0) {
            low = mid + 1;
          } else if (cmp > 0) {
            high = mid - 1;
          } else {
            arc.arc_idx = mid - 1;
            read_next_real_arc(arc, in);
            return true;
          }  // End if
        }  // End while
      }  // End if
      return false;
    }  // End if

    // Linear scan
    read_first_real_target_arc(follow.target, arc, in);

    while (true) {
      if (arc.label == label_to_match) {
        return true;
      } else if (arc.label > label_to_match || arc.is_last()) {
        return false;
      } else {
        read_next_real_arc(arc, in);
      }
    }  // End while
  }

  void read_first_real_target_arc(int64_t node_address, FstArc<T> &arc, FstReverseBytesReader &in) {
    in.seek(node_address);

    const uint8_t flags = arc.node_flags = in.read_byte();
    if (flags == FST_ARCS_FOR_BINARY_SEARCH || flags == FST_ARCS_FOR_DIRECT_ADDRESSING) {
      // Special arc which is actually a node header for fixed length arcs.
      //  case 1. Direct accessing -> label range
      //  case 2. binary search -> num arcs
      arc.num_arcs = in.read_vint();
      arc.bytes_per_arc = in.read_vint();
      arc.arc_idx = -1;
      if (flags == FST_ARCS_FOR_DIRECT_ADDRESSING) {
        read_presence_bytes(arc, in);
        arc.first_label = read_label(in);
        arc.presence_index = -1;
      }
      arc.pos_arcs_start = in.get_file_pointer();
    } else {
      arc.next_arc = node_address;
      arc.bytes_per_arc = 0;
    }  // End if

    read_next_real_arc(arc, in);
  }

  void read_next_real_arc(FstArc<T> &arc, FstReverseBytesReader &in) {
    switch (arc.node_flags) {
      case FST_ARCS_FOR_BINARY_SEARCH: {
        assert(arc.bytes_per_arc > 0);
        arc.arc_idx++;
        assert(arc.arc_idx >= 0 && arc.arc_idx < arc.num_arcs);
        in.seek(arc.pos_arcs_start - arc.arc_idx * (int64_t) arc.bytes_per_arc);
        arc.flags = in.read_byte();
        break;
      }

      case FST_ARCS_FOR_DIRECT_ADDRESSING: {
        assert(FstBitTable::assert_is_valid(arc, in));
        assert(arc.arc_idx == -1 || FstBitTable::is_bit_set(arc.arc_idx, arc, in));
        const int32_t next_index = FstBitTable::next_bit_set(arc.arc_idx, arc, in);
        read_arc_by_direct_addressing(arc, in, next_index, arc.presence_index + 1);
        return;
      }

      default: {
        // Variable length arcs - linear search.
        assert(arc.bytes_per_arc == 0);
        in.seek(arc.next_arc);
        arc.flags = in.read_byte();
      }
    }  // End switch

    read_arc(arc, in);
  }

  void read_arc(FstArc<T> &arc, FstReverseBytesReader &in) {
    if (arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING) {
      arc.label = arc.first_label + arc.arc_idx;
    } else {
      arc.label = read_label(in);
    }

    if (arc.flag(FST_BIT_ARC_HAS_OUTPUT)) {
      outputs.read_output(in, arc.output);
    } else {
      arc.output = outputs.get_no_output();
    }

    if (arc.flag(FST_BIT_ARC_HAS_FINAL_OUTPUT)) {
      outputs.read_final_output(in, arc.next_final_output);
    } else {
      arc.next_final_output = outputs.get_no_output();
    }

    if (arc.flag(FST_BIT_STOP_NODE)) {
      if (arc.flag(FST_BIT_FINAL_ARC)) {
        arc.target = FINAL_END_NODE;
      } else {
        arc.target = FST_NON_FINAL_END_NODE;
      }
      arc.next_arc = in.get_file_pointer();  // Only useful for list.
    } else if (arc.flag(FST_BIT_TARGET_NEXT)) {
      arc.next_arc = in.get_file_pointer();  // Only useful for list.
      if (!arc.flag(FST_BIT_LAST_ARC)) {
        if (arc.bytes_per_arc == 0) {
          // must scan
          seek_to_next_node(in);
        } else {
          const int64_t num_arcs =
              arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING
              ? FstBitTable::count_bits(arc, in)
              : arc.num_arcs;
          in.seek(arc.pos_arcs_start - arc.bytes_per_arc * num_arcs);
        }
      }
      arc.target = in.get_file_pointer();
    } else {
      arc.target = read_unpacked_node_target(in);
      arc.next_arc = in.get_file_pointer();  // Only useful for list.
    }  // End if
  }

  static int64_t read_unpacked_node_target(FstReverseBytesReader &in) {
    return in.read_vlong();
  }

  void seek_to_next_node(FstReverseBytesReader &in) {
    while (true) {
      const auto flags = in.read_byte();
      read_label(in);

      if (flags & FST_BIT_ARC_HAS_OUTPUT) {
        outputs.skip_output(in);
      }

      if (flags & FST_BIT_ARC_HAS_FINAL_OUTPUT) {
        outputs.skip_final_output(in);
      }

      // if (!flag(flags, FST_BIT_STOP_NODE) && !flag(flags, FST_BIT_TARGET_NEXT)) {
      if (!((flags & FST_BIT_STOP_NODE) || (flags & FST_BIT_TARGET_NEXT))) {
        read_unpacked_node_target(in);
      }

      if (flags & FST_BIT_LAST_ARC) {
        return;
      }
    }  // End while
  }

  int32_t read_label(FstReverseBytesReader &in) {
    int32_t v;
    if (InputType == 1) {
      // Unsigned byte:
      v = in.read_byte();
    } else if (InputType == 2) {
      // Unsigned short:
      v = in.read_short();
      if (version < FST_VERSION_LITTLE_ENDIAN) {
        v = (v >> 8U) | (v << 8U);
      }
    } else {
      v = in.read_vint();
    }  // End if

    return v;
  }

  void read_arc_by_direct_addressing(
      FstArc<T> &arc, FstReverseBytesReader &in, int32_t range_index, int32_t presence_index) {
    in.seek(arc.pos_arcs_start - presence_index * (int64_t) arc.bytes_per_arc);
    arc.arc_idx = range_index;
    arc.presence_index = presence_index;
    arc.flags = in.read_byte();
    read_arc(arc, in);
  }

  void read_arc_by_direct_addressing(FstArc<T> &arc, FstReverseBytesReader &in, int32_t range_index) {
    assert(FstBitTable::assert_is_valid(arc, in));
    assert(range_index >= 0 && range_index < arc.num_arcs);
    assert(FstBitTable::is_bit_set(range_index, arc, in));
    const int32_t presence_index = FstBitTable::count_bits_up_to(range_index, arc, in);
    read_arc_by_direct_addressing(arc, in, range_index, presence_index);
  }

  void read_presence_bytes(FstArc<T> &arc, FstReverseBytesReader &in) {
    assert(arc.bytes_per_arc > 0);
    assert(arc.node_flags == FST_ARCS_FOR_DIRECT_ADDRESSING);
    arc.bit_table_start = in.get_file_pointer();
    in.skip_bytes(fst_get_num_presence_bytes(arc.num_arcs));
  }

  FstReverseBytesReader get_bytes_reader() {
    assert(num_fst_bytes <= INT32_MAX);
    return FstReverseBytesReader{
        fst_in.to_read_only().data, (int32_t) num_fst_bytes
    };
  }

  //
  // For traversal
  //
  void read_next_arc(FstArc<T> &arc, FstReverseBytesReader &in) {
    if (arc.label == FST_END_LABEL) {
      // This was a fake inserted "final" arc
      if (arc.next_arc <= 0) {
        throw 13;
      }
      read_first_real_target_arc(arc.next_arc, arc, in);
    } else {
      read_next_real_arc(arc, in);
    }  // End if
  }

  void read_first_target_arc(FstArc<T> &follow, FstArc<T> &arc, FstReverseBytesReader &in) {
    if (follow.is_final()) {
      // Insert "fake" final first arc:
      arc.label = FST_END_LABEL;
      arc.output = follow.next_final_output;
      arc.flags = FST_BIT_FINAL_ARC;
      if (follow.target <= 0) {
        arc.flags |= FST_BIT_LAST_ARC;
      } else {
        // NOTE: nextArc is a node (not an address!) in this case:
        arc.next_arc = follow.target;
      }
      arc.target = FST_FINAL_END_NODE;
      arc.node_flags = arc.flags;
    } else {
      read_first_real_target_arc(follow.target, arc, in);
    }  // End if
  }

  int32_t version;
  std::optional<T> empty_output;
  std::string empty_output_bytes;
  int64_t start_node;
  int64_t num_fst_bytes;
  store::CloseableMemoryDataInput fst_in;
  Outputs outputs;
};  // FST



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_FST_HPP_
