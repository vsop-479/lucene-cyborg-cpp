#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_MULTILEVELSKIPLISTREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_MULTILEVELSKIPLISTREADER_HPP_

#include <cstdint>
#include <cmath>
#include "../util/PointerTypes.hpp"
#include "../util/LinearAllocator.hpp"
#include "../store/ReadOnlyDataInput.hpp"
#include "../store/DataInput.hpp"

namespace lucene::cyborg::codecs {

template<int32_t MaxSkipLevel, int32_t SkipInterval, int32_t SkipMultiplier, class SkipListReader>
struct MultiLevelSkipListReader {
  explicit MultiLevelSkipListReader(
      const store::DataInput<store::ReadOnlyDataInput> &skip_in,
      const int64_t _first_skip_pointer,
      const int32_t _doc_freq)
      : number_of_skip_levels(),
        last_doc(),
        doc_count(),
        last_child_pointer() {
    skip_stream[0] = skip_in;
    skip_interval[0] = SkipInterval;
    for (int32_t i = 1; i < MaxSkipLevel; ++i) {
      skip_interval[i] = skip_interval[i - 1] * SkipMultiplier;
    }  // End for

    skip_pointer[0] = _first_skip_pointer;
    doc_count = _doc_freq;
    assert(_first_skip_pointer >= 0 && _first_skip_pointer <= skip_stream[0].input.length);
    for (int32_t i = 0; i < MaxSkipLevel; ++i) {
      skip_doc[i] = 0;
    }
    for (int32_t i = 0; i < MaxSkipLevel; ++i) {
      num_skipped[i] = 0;
    }
    for (int32_t i = 0; i < MaxSkipLevel; ++i) {
      child_pointer[i] = 0;
    }

    for (int32_t i = 1; i < number_of_skip_levels; ++i) {
      skip_stream[i].input.data = nullptr;
    }
    load_skip_levels();
  }

  int32_t skip_to(const int32_t target) {
    // walk up the levels until highest level is found that has a skip
    // for this target
    int32_t level = 0;
    while (level < number_of_skip_levels - 1 && target > skip_doc[level + 1]) {
      level++;
    }

    while (level >= 0) {
      if (target > skip_doc[level]) {
        load_next_skip(level);
      } else {
        // no more skips on this level, go down one level
        if (level > 0 && last_child_pointer > skip_stream[level - 1].get_file_pointer()) {
          ((SkipListReader *) this)->seek_child(level - 1);
        }
        --level;
      }
    }  // End while

    return num_skipped[0] - skip_interval[0] - 1;
  }

  void load_next_skip(int32_t level) {
    // we have to skip, the target document is greater than the current
    // skip list entry
    ((SkipListReader *) this)->set_last_skip_data(level);

    num_skipped[level] += skip_interval[level];

    // numSkipped may overflow a signed int, so compare as unsigned.
    if (((uint32_t) num_skipped[level]) > ((uint32_t) doc_count)) {
      // this skip list is exhausted
      skip_doc[level] = INT32_MAX;
      if (number_of_skip_levels > level) number_of_skip_levels = level;
      return;
    }

    // read next skip entry
    skip_doc[level] += ((SkipListReader *) this)->read_skip_data(level, skip_stream[level]);

    if (level != 0) {
      // read the child pointer if we are not on the leaf level
      child_pointer[level] = skip_stream[level].read_vlong() + skip_pointer[level - 1];
    }
  }

  void seek_child(const int32_t level) {
    skip_stream[level].seek(last_child_pointer);
    num_skipped[level] = num_skipped[level + 1] - skip_interval[level + 1];
    skip_doc[level] = last_doc;
    if (level > 0) {
      child_pointer[level] = skip_stream[level].read_vlong() + skip_pointer[level - 1];
    }
  }

  void set_last_skip_data(const int32_t level) noexcept {
    last_doc = skip_doc[level];
    last_child_pointer = child_pointer[level];
  }

  void load_skip_levels() {
    static const auto SkipMultiplierLogBase = std::log(SkipMultiplier);

    if (doc_count <= skip_interval[0]) {
      number_of_skip_levels = 1;
    } else {
      number_of_skip_levels = 1 + std::log(doc_count / skip_interval[0]) / SkipMultiplierLogBase;
    }

    if (number_of_skip_levels > MaxSkipLevel) {
      number_of_skip_levels = MaxSkipLevel;
    }

    skip_stream[0].seek(skip_pointer[0]);

    for (int32_t i = number_of_skip_levels - 1; i > 0; --i) {
      // the length of the current level
      const int64_t length = skip_stream[0].read_vlong();

      // the start pointer of the current level
      skip_pointer[i] = skip_stream[0].get_file_pointer();

      // clone this stream, it is already at the start of the current level
      skip_stream[i] = skip_stream[0];

      // move base stream beyond the current level
      skip_stream[0].seek(skip_stream[0].get_file_pointer() + length);
    }

    // use base stream for the lowest level
    skip_pointer[0] = skip_stream[0].get_file_pointer();
  }

  int32_t number_of_skip_levels;
  int32_t last_doc;
  int32_t doc_count;
  int64_t last_child_pointer;
  store::DataInput<store::ReadOnlyDataInput> skip_stream[MaxSkipLevel];
  int64_t skip_pointer[MaxSkipLevel];
  int64_t child_pointer[MaxSkipLevel];
  int32_t num_skipped[MaxSkipLevel];
  int32_t skip_interval[MaxSkipLevel];
  int32_t skip_doc[MaxSkipLevel];
};  // MultiLevelSkipListReader



}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_MULTILEVELSKIPLISTREADER_HPP_