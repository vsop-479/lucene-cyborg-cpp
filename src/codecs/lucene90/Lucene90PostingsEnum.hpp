#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSENUM_HPP_

#include "../../index/IndexOptions.hpp"
#include "../../index/PostingsEnum.hpp"
#include "../../util/LinearAllocator.hpp"
#include "../../util/Macros.hpp"
#include "../../util/PointerTypes.hpp"
#include "../../util/TypeInfo.hpp"
#include "../../store/ReadOnlyDataInput.hpp"
#include "../../store/DataInput.hpp"
#include "Lucene90TermState.hpp"
#include "Lucene90ScoreSkipReader.hpp"
#include "ForUtil.hpp"
#include "PForUtil.hpp"
#include "PostingsReaderBuffersAndData.hpp"

namespace lucene::cyborg::codecs::v90 {

template<index::IndexOptions OptionIndex>
struct Lucene90PostingsEnum final : public index::PostingsEnum {
  static uint64_t PCO_INDEX;

  Lucene90PostingsEnum(util::ManagedPtr<util::LinearAllocator> _allocator,
                       const Lucene90TermState &_term_state,
                       PostingsEnum::Flags _flags,
                       const store::DataInput<store::ReadOnlyDataInput> &_doc_in,
                       const store::DataInput<store::ReadOnlyDataInput> &_pos_in = {},
                       const store::DataInput<store::ReadOnlyDataInput> &_pay_in = {},
                       const bool _has_payload = false)
      : PostingsEnum(util::TypeInfo::get_type_info<decltype(this)>("Lucene90PostingsEnum")),
        buffers_and_data(),
        total_term_freq(OptionIndex == index::IndexOptions::DOCS ? _term_state.doc_freq : _term_state.total_term_freq),
        accum(),
        doc_term_start_fp(_term_state.doc_start_fp),
        skip_offset(_term_state.skip_offset),
        doc_freq(_term_state.doc_freq),
        block_upto(),
        doc(-1),
        next_skip_doc(ForUtil::BLOCK_SIZE - 1),
        singleton_doc_id(_term_state.singleton_doc_id),
        skipper(),
        pfor_util(),
        doc_in(_doc_in),
        pos_in(),
        pay_in(),
        allocator(_allocator) {
    pco_type_index = PCO_INDEX;

    buffers_and_data.doc_buffer_upto = ForUtil::BLOCK_SIZE;

    if (_term_state.doc_freq > 1) {
      doc_in.seek(_term_state.doc_start_fp);
    }
    buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE] = NO_MORE_DOCS;

    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
      buffers_and_data.is_freqs_read = true;

      if (_flags >= PostingsEnum::Flags::FREQS) {
        // Use freqs
        buffers_and_data.freq_buffer = _allocator->allocate_objects<int64_t>(ForUtil::BLOCK_SIZE);
      }
    }

    if constexpr (OptionIndex > index::IndexOptions::DOCS_AND_FREQS) {
      if (_flags >= PostingsEnum::Flags::POSITIONS) {
        // TODO : init pos
        // TODO : init offsets
        // TODO : init payloads
        assert(false);
      }
    }
  }

  int32_t get_doc_id() final {
    return doc;
  }

  int32_t get_freq() final {
    if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
      if (buffers_and_data.is_freqs_read) {
        return buffers_and_data.freq_buffer[buffers_and_data.doc_buffer_upto - 1];
      } else {
        pfor_util.skip_freq(doc_in);
        buffers_and_data.is_freqs_read = true;
        return buffers_and_data.freq_buffer[buffers_and_data.doc_buffer_upto - 1];
      }
    }
  }

  int32_t next_doc() final {
    if (buffers_and_data.doc_buffer_upto != ForUtil::BLOCK_SIZE) {
      NEXT_DOC:
      return doc = buffers_and_data.doc_buffer[buffers_and_data.doc_buffer_upto++];
    } else {
      refill_docs();
      goto NEXT_DOC;
    }  // End if
  }

  int64_t get_cost() final {
    return doc_freq;
  }

  LC_ALWAYSINLINE int32_t advance(int32_t target) final {
    // current skip docID < docIDs generated from current buffer <= next skip docID
    // we don't need to skip if target is buffered already
    if (doc_freq > ForUtil::BLOCK_SIZE && target > next_skip_doc) [[unlikely]] {
      skip_if(target);
    }

    if (buffers_and_data.doc_buffer_upto == ForUtil::BLOCK_SIZE) [[unlikely]] {
      refill_docs();
    }

    // Now scan... this is an inlined/pared down version
    // of next_doc():
    int32_t doc_buffer_upto = buffers_and_data.doc_buffer_upto;
    int64_t doc;
    while (true) {
      LC_PREFETCH(&buffers_and_data.doc_buffer[doc_buffer_upto], 0, 0);
      doc = buffers_and_data.doc_buffer[doc_buffer_upto];
      if (doc < target) {
        ++doc_buffer_upto;
      } else {
        break;
      }
    }  // End while

    buffers_and_data.doc_buffer_upto = doc_buffer_upto + 1;
    return this->doc = (int32_t) doc;
  }

  void refill_docs() {
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
          buffers_and_data.is_freqs_read = false;
        } else {
          pfor_util.skip_freq(doc_in); // skip over freq_buffer if we don't need them at all
        }
      }

      block_upto += ForUtil::BLOCK_SIZE;
    } else if (doc_freq == 1) {
      buffers_and_data.doc_buffer[0] = singleton_doc_id;
      buffers_and_data.doc_buffer[1] = DocIdSetIterator::NO_MORE_DOCS;
      if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
        if (buffers_and_data.freq_buffer) {
          buffers_and_data.freq_buffer[0] = total_term_freq;
        } else {
          buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE + 1] = singleton_doc_id;
        }
      } else {
        buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE + 1] = singleton_doc_id;
      }

      ++block_upto;
    } else {
      if constexpr (OptionIndex == index::IndexOptions::DOCS) {
        read_vint_block<index::IndexOptions::DOCS, true>(doc_in,
                                                         buffers_and_data.doc_buffer,
                                                         left);
      } else {
        if (buffers_and_data.freq_buffer) {
          read_vint_block<OptionIndex, false>(doc_in,
                                              buffers_and_data.doc_buffer,
                                              left,
                                              buffers_and_data.freq_buffer);
        } else {
          read_vint_block<OptionIndex, true>(doc_in,
                                             buffers_and_data.doc_buffer,
                                             left,
                                             nullptr);
        }
      }
      doc_id_prefix_sum(buffers_and_data.doc_buffer, left, accum);
      buffers_and_data.doc_buffer[left] = DocIdSetIterator::NO_MORE_DOCS;
      block_upto += left;
    }  // End if

    accum = buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE - 1];
    buffers_and_data.doc_buffer_upto = 0;
    assert(buffers_and_data.doc_buffer[ForUtil::BLOCK_SIZE] == DocIdSetIterator::NO_MORE_DOCS);
  }

  void skip_if(int32_t target) {
    if (skipper) [[likely]] {
      SKIP_PART:
      // always plus one to fix the result, since skip position in Lucene90SkipReader
      // is a little different from MultiLevelSkipListReader
      const int32_t new_doc_upto = skipper->skip_to(target) + 1;

      if (new_doc_upto >= block_upto) {
        // Skipper moved
        assert(new_doc_upto % ForUtil::BLOCK_SIZE == 0);
        block_upto = new_doc_upto;

        // Force to read next block
        buffers_and_data.doc_buffer_upto = ForUtil::BLOCK_SIZE;
        accum = skipper->last_doc; // actually, this is just last_skip_entry
        doc_in.seek(skipper->buffer_and_pointers.last_doc_pointer); // now point to the block we want to search

        if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
          buffers_and_data.is_freqs_read = true;
        }
      }  // End if

      // next time we call advance, this is used to
      // foresee whether skipper is necessary.
      next_skip_doc = skipper->get_next_skip_doc();
    } else {
      // Lazy init: first time this enum has ever been used for skipping
      skipper =
          allocator->allocate_object<Lucene90SkipReader<OptionIndex>>(
              allocator, doc_in, doc_freq, doc_term_start_fp + skip_offset, doc_term_start_fp);
      assert(skip_offset != -1);
      goto SKIP_PART;
    }  // End if
  }

  PostingsReaderBuffersAndData<OptionIndex> buffers_and_data;
  int64_t total_term_freq;
  int64_t accum;
  int64_t doc_term_start_fp;
  int64_t skip_offset;
  int32_t doc_freq;
  int32_t block_upto;
  int32_t doc;
  int32_t next_skip_doc;
  int32_t singleton_doc_id;
  util::ManagedPtr<Lucene90SkipReader<OptionIndex>> skipper;
  PForUtil pfor_util;
  store::DataInput<store::ReadOnlyDataInput> doc_in;
  store::DataInput<store::ReadOnlyDataInput> pos_in;
  store::DataInput<store::ReadOnlyDataInput> pay_in;
  util::ManagedPtr<util::LinearAllocator> allocator;
};  // Lucene90PostingsEnum

}  // namespace lucene::cyborg::::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSENUM_HPP_
