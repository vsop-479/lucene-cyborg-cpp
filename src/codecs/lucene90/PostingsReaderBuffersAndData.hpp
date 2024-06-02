#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_POSTINGSREADERBUFFERSANDDATA_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_POSTINGSREADERBUFFERSANDDATA_HPP_

#include "../../index/IndexOptions.hpp"
#include "../../store/DataInput.hpp"
#include "../../util/PointerTypes.hpp"
#include "ForUtil.hpp"

namespace lucene::cyborg::codecs::v90 {

inline void doc_id_prefix_sum(util::UseOnlyPtr<int64_t> doc_buffer, const int32_t count, const int64_t base) {
  doc_buffer[0] += base;
  if (count > 0) {
    auto prev = &doc_buffer[0];
    auto next = &doc_buffer[1];
    auto until = &doc_buffer[count];
    while (next != until) {
      *next++ += *prev++;
    }
  }  // End if
}

template<index::IndexOptions OptionIndex, bool SkipTermFreqs>
void read_vint_block(store::DataInput<store::ReadOnlyDataInput> &doc_in,
                     util::UseOnlyPtr<int64_t> doc_buffer,
                     const int32_t num,
                     util::UseOnlyPtr<int64_t> freq_buffer = nullptr) {
  if constexpr (OptionIndex >= index::IndexOptions::DOCS_AND_FREQS) {
    for (int32_t i = 0; i < num; ++i) {
      const uint32_t code = doc_in.read_vint();
      doc_buffer[i] = code >> 1U;
      if constexpr (SkipTermFreqs) {
        if (code & 1U) {
          // TF = 1
        } else {
          // TF > 1
          doc_in.skip_variant_integer();
        }
      } else {
        assert(freq_buffer);  // Requested freq, but no buffer? does not make any sense.
        if (code & 1U) {
          // TF = 1
          freq_buffer[i] = 1;
        } else {
          // TF > 1
          freq_buffer[i] = doc_in.read_vint();
        }
      }
    }  // End for
  } else {
    for (int32_t i = 0; i < num; i++) {
      doc_buffer[i] = doc_in.read_vint();
    }
  }  // End if
}

template<index::IndexOptions OptionsIndex>
struct PostingsReaderBuffersAndData {
  // Use the last int64_t of `doc_buffer` (e.g. at ForUtil::BLOCK_SIZE + 1) as a freq value when |docs| == 1
  int64_t doc_buffer[ForUtil::BLOCK_SIZE + 2];
  int32_t doc_buffer_upto;

  util::ManagedPtr<int64_t> freq_buffer;
  int32_t freq;
  bool is_freqs_read;

  util::ManagedPtr<int64_t> pos_delta_buffer;
  util::ManagedPtr<int64_t> payload_length_buffer;
  util::ManagedPtr<int64_t> offset_start_delta_buffer;
  util::ManagedPtr<int64_t> offset_length_buffer;
  util::ManagedPtr<uint8_t> payload_bytes;
  int32_t position;

  // how many positions "behind" we are; nextPosition must
  // skip these to "catch up":
  int32_t pos_pending_count;

  // Lazy pos seek: if != -1 then we must seek to this FP
  // before reading positions:
  int64_t pos_pending_fp;

  // Lazy pay seek: if != -1 then we must seek to this FP
  // before reading payloads/offsets:
  int64_t pay_pending_fp;

  // Where this term's postings start in the .pos file:
  int64_t pos_term_start_f_p;

  // Where this term's payloads/offsets start in the .pay file
  int64_t pay_term_start_fp;

  // File pointer where the last (vInt encoded) pos delta block is.
  // We need this to know whether to bulk decode vs vInt decode the block
  int64_t last_pos_block_fp;

  bool index_has_offsets;
  bool index_has_payloads;
};  // PostingsReaderBuffersAndData


template<>
struct PostingsReaderBuffersAndData<index::IndexOptions::DOCS> {
  // Use the last int64_t of `doc_buffer` (e.g. at ForUtil::BLOCK_SIZE + 1) as a freq value when |docs| == 1
  int64_t doc_buffer[ForUtil::BLOCK_SIZE + 2];
  int32_t doc_buffer_upto;
};  // PostingsReaderBuffersAndData<IndexOptions::DOCS>



template<>
struct PostingsReaderBuffersAndData<index::IndexOptions::DOCS_AND_FREQS> final
    : public PostingsReaderBuffersAndData<index::IndexOptions::DOCS> {
  util::ManagedPtr<int64_t> freq_buffer;
  int32_t freq;
  bool is_freqs_read;
};  // PostingsReaderBuffersAndData<IndexOptions::DOCS_AND_FREQS>



}  // namespace lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_POSTINGSREADERBUFFERSANDDATA_HPP_
