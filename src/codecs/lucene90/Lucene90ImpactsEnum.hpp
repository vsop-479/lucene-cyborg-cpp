#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90IMPACTSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90IMPACTSENUM_HPP_

#include "../../index/IndexOptions.hpp"
#include "../../index/ImpactsEnum.hpp"
#include "../../util/LinearAllocator.hpp"
#include "../../store/ReadOnlyDataInput.hpp"
#include "../../store/DataInput.hpp"
#include "Lucene90TermState.hpp"
#include "Lucene90ScoreSkipReader.hpp"
#include "ForUtil.hpp"
#include "PForUtil.hpp"
#include "PostingsReaderBuffersAndData.hpp"

namespace lucene::cyborg::codecs::v90 {

template<index::IndexOptions OptionIndex>
struct Lucene90ImpactsEnum final : public index::ImpactsEnum {
  static uint64_t PCO_INDEX;

  Lucene90ImpactsEnum(index::IndexOptions _index_options,
                      util::ManagedPtr<util::LinearAllocator> _allocator,
                      const Lucene90TermState &_term_state,
                      PostingsEnum::Flags _flags,
                      const store::DataInput<store::ReadOnlyDataInput> &_doc_in,
                      const store::DataInput<store::ReadOnlyDataInput> &_pos_in = {},
                      const store::DataInput<store::ReadOnlyDataInput> &_pay_in = {},
                      const bool _has_payload = false)
      : ImpactsEnum(util::TypeInfo::get_type_info<decltype(this)>("Lucene90ImpactsEnum")),
        buffers_and_data(),
        accum(),
        seek_to(-1),
        doc_freq(_term_state.doc_freq),
        block_upto(),
        doc(-1),
        next_skip_doc(-1),
        skipper(),
        pfor_util(),
        doc_in(_doc_in),
        pos_in(_pos_in),
        pay_in(_pay_in),
        allocator(_allocator) {
    pco_type_index = PCO_INDEX;

    buffers_and_data.doc_buffer_upto = ForUtil::BLOCK_SIZE;

    // Move to postings list section
    doc_in.seek(_term_state.doc_start_fp);

    // Skip list accessor
    // TODO : support pos + offset + payload
    skipper = _allocator->allocate_object<Lucene90ScoreSkipReader<OptionIndex>>(
        allocator, doc_in, doc_freq, _term_state.doc_start_fp + _term_state.skip_offset, _term_state.doc_start_fp);

    // We set the last element of doc_buffer to NO_MORE_DOCS, it helps save conditionals in advance()
    buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE] = NO_MORE_DOCS;

    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
      buffers_and_data.is_freqs_read = true;
      if (_flags >= PostingsEnum::Flags::FREQS) {
        // Client required term freq
        buffers_and_data.freq_buffer = _allocator->allocate_objects<int64_t>(ForUtil::BLOCK_SIZE);
      }
    }  // End if
  }

  int32_t get_doc_id() final {
    return doc;
  }

  int32_t get_freq() final {
    if constexpr (OptionIndex == index::IndexOptions::DOCS) {
      return 1;
    } else if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
      if (buffers_and_data.freq_buffer) {
        // Client has requested term freqs

        if (buffers_and_data.is_freqs_read) {
          FREQ_BUFFER_ACCESS:
          return (int32_t) buffers_and_data.freq_buffer[buffers_and_data.doc_buffer_upto - 1];
        } else {
          pfor_util.decode(doc_in, buffers_and_data.freq_buffer); // read freqBuffer for this block
          buffers_and_data.is_freqs_read = true;
          goto FREQ_BUFFER_ACCESS;
        }
      } else {
        // Although index has freq inside, returning 1 as client has not requested it at the beginning.
        return 1;
      }
    }  // End if
  }

  void refill_docs() {
    // Check if we skipped reading the previous block of freq_buffer, and if yes, position doc_in after it
    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
      if (!buffers_and_data.is_freqs_read) {
        pfor_util.skip_freq(doc_in);
        buffers_and_data.is_freqs_read = true;
      }
    }

    const int32_t left = doc_freq - block_upto;
    assert(left >= 0);

    if (left >= ForUtil::BLOCK_SIZE) {
      pfor_util.decode_and_prefix_sum(doc_in, accum, buffers_and_data.doc_buffer);
      if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
        if (buffers_and_data.freq_buffer) {
          pfor_util.decode(doc_in, buffers_and_data.freq_buffer);
        } else {
          PForUtil::skip_freq(doc_in);
        }
      }

      block_upto += ForUtil::BLOCK_SIZE;
    } else {
      if constexpr (OptionIndex == index::IndexOptions::DOCS) {
        read_vint_block<index::IndexOptions::DOCS, true>(doc_in, buffers_and_data.doc_buffer, left);
      } else {
        if (buffers_and_data.freq_buffer) {
          read_vint_block<OptionIndex, false>(doc_in, buffers_and_data.doc_buffer, left, buffers_and_data.freq_buffer);
        } else {
          read_vint_block<OptionIndex, true>(doc_in, buffers_and_data.doc_buffer, left);
        }
      }
      doc_id_prefix_sum(buffers_and_data.doc_buffer, left, accum);
      buffers_and_data.doc_buffer[left] = NO_MORE_DOCS;
      block_upto += left;
    }
    accum = buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE - 1];
    buffers_and_data.doc_buffer_upto = 0;
    assert(buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE] == NO_MORE_DOCS);
  }

  void advance_shallow(int32_t target) final {
    if (target > next_skip_doc) {
      // Always plus one to fix the result, since skip position in Lucene90SkipReader
      // is a little different from MultiLevelSkipListReader
      const int32_t new_doc_upto = skipper->skip_to(target) + 1;

      if (new_doc_upto >= block_upto) {
        // Skipper moved
        assert(new_doc_upto % ForUtil::BLOCK_SIZE == 0);
        block_upto = new_doc_upto;

        // Force to read next block
        buffers_and_data.doc_buffer_upto = ForUtil::BLOCK_SIZE;
        accum = skipper->last_doc;
        seek_to = skipper->buffer_and_pointers.last_doc_pointer; // Delay the seek
      }
      // Next time we call advance, this is used to
      // foresee whether skipper is necessary.
      next_skip_doc = skipper->get_next_skip_doc();
    }
    assert(next_skip_doc >= target);
  }

  util::ManagedPtr<Lucene90Impacts> get_impacts() final {
    advance_shallow(doc);
    return skipper->get_impacts();
  }

  int32_t next_doc() final {
    return advance(doc + 1);
  }

  int32_t advance(int32_t target) final {
    if (target > next_skip_doc) {
      advance_shallow(target);
    }
    if (buffers_and_data.doc_buffer_upto == ForUtil::BLOCK_SIZE) {
      if (seek_to >= 0) {
        doc_in.seek(seek_to);
        seek_to = -1;

        if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
          buffers_and_data.is_freqs_read = true; // reset is_freqs_read
        }
      }
      refill_docs();
    }

    const int32_t next = find_first_greater(buffers_and_data.doc_buffer, target, buffers_and_data.doc_buffer_upto);
    doc = (int32_t) buffers_and_data.doc_buffer[next];
    buffers_and_data.doc_buffer_upto = next + 1;
    return doc;
  }

  // TODO
  // public int next_position() final {
  //     return -1;
  // }
  // TODO
  // int start_offset() final {
  //   return -1;
  // }
  //
  // TODO
  // int end_offset() final {
  //   return -1;
  // }
  //
  // TODO
  // BytesRef get_payload() final {
  //  return null;
  // }

  int64_t get_cost() final {
    return doc_freq;
  }

  static int32_t find_first_greater(util::ReadOnlyPtr<int64_t> buffer, const int32_t target, const int32_t from) {
    for (int32_t i = from; i < ForUtil::BLOCK_SIZE; ++i) {
      if (buffer[i] >= target) {
        return i;
      }
    }  // End for

    return ForUtil::BLOCK_SIZE;
  }

  PostingsReaderBuffersAndData<OptionIndex> buffers_and_data;
  int64_t accum;
  int64_t seek_to;
  int32_t doc_freq;
  int32_t block_upto;
  int32_t doc;
  int32_t next_skip_doc;
  util::ManagedPtr<Lucene90ScoreSkipReader<OptionIndex>> skipper;
  PForUtil pfor_util;
  store::DataInput<store::ReadOnlyDataInput> doc_in;
  store::DataInput<store::ReadOnlyDataInput> pos_in;
  store::DataInput<store::ReadOnlyDataInput> pay_in;
  util::ManagedPtr<util::LinearAllocator> allocator;
};  // Lucene90ImpactsEnum

}  // namespace lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90IMPACTSENUM_HPP_
