#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SKIPREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SKIPREADER_HPP_

#include "../MultiLevelSkipListReader.hpp"
#include "Lucene90Constants.hpp"
#include "ForUtil.hpp"
#include "../../index/IndexOptions.hpp"
#include "../../index/PostingsEnum.hpp"

namespace lucene::cyborg::codecs::v90 {

//
// Lucene90SkipReader
//


template<index::IndexOptions OptionIndex>
struct Lucene90SkipReaderBufferAndPointers {
  int64_t doc_pointer[Lucene90Constants::MAX_SKIP_LEVELS];
  util::ManagedPtr<int64_t> pos_pointer;
  util::ManagedPtr<int64_t> pos_buffer_upto;
  util::ManagedPtr<int64_t> pay_pointer;
  util::ManagedPtr<int64_t> pay_buffer_upto;
  int64_t last_doc_pointer;
  int64_t last_pos_pointer;
  int64_t last_pos_buffer_upto;
  int64_t last_pay_pointer;
  int64_t last_pay_buffer_upto;
};

template<>
struct Lucene90SkipReaderBufferAndPointers<index::IndexOptions::DOCS> {
  int64_t doc_pointer[Lucene90Constants::MAX_SKIP_LEVELS];
  int64_t last_doc_pointer;
};

template<>
struct Lucene90SkipReaderBufferAndPointers<index::IndexOptions::DOCS_AND_FREQS> {
  int64_t doc_pointer[Lucene90Constants::MAX_SKIP_LEVELS];
  int64_t last_doc_pointer;
};

template<index::IndexOptions OptionIndex>
struct Lucene90SkipReader :
    public MultiLevelSkipListReader<Lucene90Constants::MAX_SKIP_LEVELS,
                                    ForUtil::BLOCK_SIZE,
                                    Lucene90Constants::SKIP_MULTIPLIER,
                                    Lucene90SkipReader<OptionIndex>> {
  using Super = MultiLevelSkipListReader<Lucene90Constants::MAX_SKIP_LEVELS,
                                         ForUtil::BLOCK_SIZE,
                                         Lucene90Constants::SKIP_MULTIPLIER,
                                         Lucene90SkipReader<OptionIndex>>;

  explicit Lucene90SkipReader(util::UseOnlyPtr<util::LinearAllocator> _allocator,
                              const store::DataInput<store::ReadOnlyDataInput> &_skip_in,
                              const int32_t _df,
                              const int64_t _first_skip_pointer,
                              const int64_t _doc_base_pointer,
                              const int64_t _pos_base_pointer = -1,
                              const bool _has_payload = false,
                              const int64_t _pay_base_pointer = -1)
      : Super(_skip_in, _first_skip_pointer, trim(_df)),
        has_payload(_has_payload),
        buffer_and_pointers() {
    buffer_and_pointers.last_doc_pointer = _doc_base_pointer;
    for (int32_t i = 0; i < Lucene90Constants::MAX_SKIP_LEVELS; ++i) {
      buffer_and_pointers.doc_pointer[i] = _doc_base_pointer;
    }

    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
      if (_pos_base_pointer > 0) {
        // Use positions
        buffer_and_pointers.last_pos_pointer = _pos_base_pointer;
        buffer_and_pointers.pos_pointer = _allocator->allocate_objects<int64_t>(Lucene90Constants::MAX_SKIP_LEVELS);
        for (int32_t i = 0; i < Lucene90Constants::MAX_SKIP_LEVELS; ++i) {
          buffer_and_pointers.pos_pointer[i] = _pos_base_pointer;
        }
        buffer_and_pointers.pos_buffer_upto = _allocator->allocate_objects<int64_t>(Lucene90Constants::MAX_SKIP_LEVELS);

        if (_pay_base_pointer > 0) {
          // Use payloads
          assert(_has_payload);
          buffer_and_pointers.last_pay_pointer = _pay_base_pointer;
          buffer_and_pointers.pay_pointer = _allocator->allocate_objects<int64_t>(Lucene90Constants::MAX_SKIP_LEVELS);
          for (int32_t i = 0; i < Lucene90Constants::MAX_SKIP_LEVELS; ++i) {
            buffer_and_pointers.pay_pointer[i] = _pay_base_pointer;
          }
          buffer_and_pointers.pay_buffer_upto =
              _allocator->allocate_objects<int64_t>(Lucene90Constants::MAX_SKIP_LEVELS);
        }
      }  // End if
    } else {
      assert(_pos_base_pointer == -1);
    }  // End if
  }

  /**
   * Trim original docFreq to tell skip_reader read proper number of skip points.
   *
   * <p>Since our definition in Lucene90Skip* is a little different from MultiLevelSkip* This
   * trimmed doc_freq will prevent skip_reader from:
   *   1. silly reading a non-existed skip point after the last block boundary
   *   2. moving into the v_int block
   */
  static constexpr int32_t trim(int32_t df) noexcept {
    return (df % ForUtil::BLOCK_SIZE == 0) ? (df - 1) : df;
  }

  void seek_child(const int32_t level) {
    Super::seek_child(level);
    buffer_and_pointers.doc_pointer[level] = buffer_and_pointers.last_doc_pointer;
    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
      if (buffer_and_pointers.pos_pointer) {
        buffer_and_pointers.pos_pointer[level] = buffer_and_pointers.last_pos_pointer;
        buffer_and_pointers.pos_buffer_upto[level] = buffer_and_pointers.last_pos_buffer_upto;

        if (buffer_and_pointers.pay_pointer) {
          buffer_and_pointers.pay_pointer[level] = buffer_and_pointers.last_pay_pointer;
          buffer_and_pointers.pay_buffer_upto[level] = buffer_and_pointers.last_pay_buffer_upto;
        }
      }
    }  // End if
  }

  void set_last_skip_data(const int32_t level) {
    Super::set_last_skip_data(level);
    buffer_and_pointers.last_doc_pointer = buffer_and_pointers.doc_pointer[level];

    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
      if (buffer_and_pointers.pos_pointer) {
        buffer_and_pointers.last_pos_pointer = buffer_and_pointers.pos_pointer[level];
        buffer_and_pointers.last_pos_buffer_upto = buffer_and_pointers.pos_buffer_upto[level];

        if (buffer_and_pointers.pay_pointer) {
          buffer_and_pointers.last_pay_pointer = buffer_and_pointers.pay_pointer[level];
          buffer_and_pointers.last_pay_buffer_upto = buffer_and_pointers.pay_buffer_upto[level];
        }
      }
    }  // End if
  }

  int32_t get_next_skip_doc() const noexcept {
    return Super::skip_doc[0];
  }

  int32_t read_skip_data(const int32_t level, store::DataInput<store::ReadOnlyDataInput> &skip_in) {
    const auto delta = skip_in.read_vint();
    buffer_and_pointers.doc_pointer[level] += skip_in.read_vlong();

    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
      if (buffer_and_pointers.pos_pointer) {
        buffer_and_pointers.pos_pointer[level] += skip_in.read_vlong();
        buffer_and_pointers.pos_buffer_upto[level] = skip_in.read_vint();
      } else {
        skip_in.skip_variant_integer();
        skip_in.skip_variant_integer();
      }

      if (has_payload) {
        if (buffer_and_pointers.pay_pointer) {
          buffer_and_pointers.pay_buffer_upto[level] = skip_in.read_vint();
          buffer_and_pointers.pay_pointer[level] += skip_in.read_vlong();
        } else {
          skip_in.skip_variant_integer();
          skip_in.skip_variant_integer();
        }
      }
    }  // End if

    read_impacts(level, skip_in);
    return delta;
  }

  virtual void read_impacts(const int32_t level, store::DataInput<store::ReadOnlyDataInput> &skip_in) {
    const int32_t skip_bytes = skip_in.read_vint();
    skip_in.skip_bytes(skip_bytes);
  }

  bool has_payload;
  Lucene90SkipReaderBufferAndPointers<OptionIndex> buffer_and_pointers;
};  // Lucene90SkipReaderBase



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SKIPREADER_HPP_
