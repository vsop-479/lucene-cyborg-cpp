#include "Lucene90PostingsFormat.hpp"
#include "Lucene90PostingsReader.hpp"
#include "PForUtil.hpp"
#include "Lucene90ScoreSkipReader.hpp"
#include "../../index/SlowImpactsEnum.hpp"
#include "Lucene90ImpactsEnum.hpp"
#include "Lucene90PostingsEnum.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::v90::ForUtil;
using lucene::cyborg::codecs::v90::Lucene90Constants;
using lucene::cyborg::codecs::v90::Lucene90Impacts;
using lucene::cyborg::codecs::v90::Lucene90ImpactsEnum;
using lucene::cyborg::codecs::v90::Lucene90PostingsEnum;
using lucene::cyborg::codecs::v90::Lucene90PostingsFormat;
using lucene::cyborg::codecs::v90::Lucene90PostingsReader;
using lucene::cyborg::codecs::v90::Lucene90PostingsReaderPtr;
using lucene::cyborg::codecs::v90::Lucene90ScoreSkipReader;
using lucene::cyborg::codecs::v90::Lucene90SkipReader;
using lucene::cyborg::codecs::v90::Lucene90TermState;
using lucene::cyborg::codecs::v90::PForUtil;
using lucene::cyborg::codecs::v90::PostingsReaderBuffersAndData;
using lucene::cyborg::codecs::v90::doc_id_prefix_sum;
using lucene::cyborg::codecs::v90::read_vint_block;
using lucene::cyborg::index::FieldInfo;
using lucene::cyborg::index::Impacts;
using lucene::cyborg::index::ImpactsEnum;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::SegmentReadState;
using lucene::cyborg::index::SlowImpactsEnum;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::DataDecoding;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::UseOnlyPtr;

static const std::string TERMS_CODEC = "Lucene90PostingsWriterTerms";
static const std::string DOC_CODEC = "Lucene90PostingsWriterDoc";
static const std::string POS_CODEC = "Lucene90PostingsWriterPos";
static const std::string PAY_CODEC = "Lucene90PostingsWriterPay";
static const int32_t VERSION_START = 0;
static const int32_t VERSION_CURRENT = VERSION_START;

int32_t check_integrity(const std::string &file_name,
                        ReadOnlyPtr<SegmentReadState> state,
                        const std::string &codec_name,
                        int32_t min_version,
                        int32_t max_version) {
  const auto file_length = state->directory->file_length(file_name);
  int32_t actual_version = -1;
  {
    const int64_t header_map_size = std::min(file_length, CodecUtil::estimate_header_size(codec_name.size()));
    auto header_in = state->directory->open_input_with_region(file_name, 0, header_map_size);
    auto header_read_only = header_in.to_read_only();
    auto header_checksum = DataInput<BufferedChecksumMemoryDataInput>{
        BufferedChecksumMemoryDataInput{
            header_read_only.data,
            header_read_only.offset,
            header_read_only.length,
            BufferedChecksum{}
        }
    };
    actual_version = CodecUtil::check_index_header(
        header_checksum,
        codec_name,
        min_version,
        max_version,
        state->segment_info->id,
        state->segment_suffix);
  }

  auto footer_in = state->directory
      ->open_input_with_region(file_name, file_length - CodecUtil::footer_length(), CodecUtil::footer_length());
  auto footer_read_only = footer_in.to_read_only();
  auto footer_checksum = DataInput<BufferedChecksumMemoryDataInput>{
      BufferedChecksumMemoryDataInput{
          footer_read_only.data,
          footer_read_only.offset,
          footer_read_only.length,
          BufferedChecksum{}
      }
  };
  CodecUtil::retrieve_checksum(footer_checksum);
  return actual_version;
}



//
// Lucene90PostingsReader
//

Lucene90PostingsReader::Lucene90PostingsReader(ReadOnlyPtr<SegmentReadState> _state)
    : PostingsReaderBase(),
      doc_file_size(),
      pos_file_size(),
      pay_file_size(),
      doc_file_name(),
      pos_file_name(),
      pay_file_name(),
      doc_input(),
      pos_input(),
      pay_input() {
  // NOTE: these data files are too costly to verify checksum against all the bytes on open,
  // but for now we at least verify proper structure of the checksum footer: which looks
  // for FOOTER_MAGIC + algorithmID. This is cheap and can detect some forms of corruption
  // such as file truncation.

  this->doc_file_name =
      IndexFileNames::segment_file_name(
          _state->segment_info->name,
          _state->segment_suffix,
          Lucene90PostingsFormat::DOC_EXTENSION);

  const int32_t actual_version = check_integrity(doc_file_name,
                                                 _state,
                                                 DOC_CODEC,
                                                 VERSION_START,
                                                 VERSION_CURRENT);

  doc_file_size = _state->directory->file_length(doc_file_name);

  if (_state->field_infos->has_prox) {
    this->pos_file_name =
        IndexFileNames::segment_file_name(
            _state->segment_info->name, _state->segment_suffix, Lucene90PostingsFormat::POS_EXTENSION);

    check_integrity(pos_file_name,
                    _state,
                    POS_CODEC,
                    actual_version,
                    actual_version);

    pos_file_size = _state->directory->file_length(pos_file_name);

    if (_state->field_infos->has_payloads || _state->field_infos->has_offsets) {
      this->pay_file_name =
          IndexFileNames::segment_file_name(
              _state->segment_info->name,
              _state->segment_suffix,
              Lucene90PostingsFormat::PAY_EXTENSION);

      check_integrity(pay_file_name,
                      _state,
                      PAY_CODEC,
                      actual_version,
                      actual_version);

      pay_file_size = _state->directory->file_length(pay_file_name);
    }
  }  // End if
}

Lucene90PostingsReader::Lucene90PostingsReader(const std::string &_doc_file_name,
                                               CloseableMemoryDataInput _doc_input,
                                               const std::string &_pos_file_name,
                                               CloseableMemoryDataInput _pos_input,
                                               const std::string &_pay_file_name,
                                               CloseableMemoryDataInput _pay_input)
    : PostingsReaderBase(),
      doc_file_size(),
      pos_file_size(),
      pay_file_size(),
      doc_file_name(_doc_file_name),
      pos_file_name(_pos_file_name),
      pay_file_name(_pay_file_name),
      doc_input(std::move(_doc_input)),
      pos_input(std::move(_pos_input)),
      pay_input(std::move(_pay_input)) {
}

void Lucene90PostingsReader::init(DataInput<BufferedChecksumMemoryDataInput> &terms_in,
                                  ReadOnlyPtr<SegmentReadState> state) {
  CodecUtil::check_index_header(terms_in,
                                TERMS_CODEC,
                                VERSION_START,
                                VERSION_CURRENT,
                                state->segment_info->id,
                                state->segment_suffix);
  const int32_t index_block_size = terms_in.read_vint();
  if (index_block_size != ForUtil::BLOCK_SIZE) {
    // TODO
    throw 13;
  }
}

template<bool has_positions, bool has_offsets, bool has_payloads>
void decode_pos_offset(DataInput<ReadOnlyDataInput> &in, Lucene90TermState &term_state) {
  if (has_positions) {
    term_state.pos_start_fp += in.read_vlong();
    if (has_offsets || has_payloads) {
      term_state.pay_start_fp += in.read_vlong();
    }
    if (term_state.total_term_freq > ForUtil::BLOCK_SIZE) {
      term_state.last_pos_block_offset = in.read_vlong();
    } else {
      term_state.last_pos_block_offset = -1;
    }
  }  // End if
}

void Lucene90PostingsReader::decode_term(DataInput<ReadOnlyDataInput> &in,
                                         Lucene90TermState &term_state) {
  const uint64_t l = in.read_vlong();
  if ((l & 0x01U) == 0) {
    term_state.doc_start_fp += l >> 1U;
    if (term_state.doc_freq == 1) {
      term_state.singleton_doc_id = in.read_vint();
    } else {
      term_state.singleton_doc_id = -1;
    }
  } else {
    assert(term_state.singleton_doc_id != -1);
    term_state.singleton_doc_id += DataDecoding::zig_zag_decode(l >> 1U);
  }  // End if

  // doc | doc + tf | pos | offset | payload
  auto advanced_options = term_state.index_options & (~(3U));
  if (advanced_options == 4U) {
    // only pos
    decode_pos_offset<true, false, false>(in, term_state);
  } else if (advanced_options == 12U) {
    // pos + offset
    decode_pos_offset<true, true, false>(in, term_state);
  } else if (advanced_options == 28U) {
    // pos + offset + payload, all
    decode_pos_offset<true, true, true>(in, term_state);
  }

  if (term_state.doc_freq > ForUtil::BLOCK_SIZE) {
    term_state.skip_offset = in.read_vlong();
  } else {
    term_state.skip_offset = -1;
  }  // End if
}

void Lucene90PostingsReader::format_term_state(
    ManagedPtr<FieldInfo> field_info,
    Lucene90TermState &term_state) {
  const uint8_t field_has_doc =
      field_info->index_options >= IndexOptions::DOCS;
  const uint8_t field_has_doc_freq =
      field_info->index_options >= IndexOptions::DOCS_AND_FREQS;
  const uint8_t field_has_positions =
      field_info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS;
  const uint8_t field_has_offsets =
      field_info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS;
  const uint8_t field_has_payloads = field_info->has_payloads();

  term_state.index_options =
      (uint8_t) ((field_has_payloads << 4U)
          | (field_has_offsets << 3U)
          | (field_has_positions << 2U)
          | (field_has_doc_freq << 1U)
          | field_has_doc);
}

ManagedPtr<PostingsEnum> Lucene90PostingsReader::postings(ManagedPtr<LinearAllocator> allocator,
                                                          ManagedPtr<FieldInfo> field_info,
                                                          const Lucene90TermState &term_state,
                                                          PostingsEnum::Flags flags) {
  const auto index_options = field_info->index_options;

  if (index_options == IndexOptions::DOCS) {
    return allocator->allocate_object<Lucene90PostingsEnum<IndexOptions::DOCS>>(
        allocator,
        term_state,
        flags,
        DataInput<ReadOnlyDataInput>{doc_input.to_read_only()});
  } else if (index_options == IndexOptions::DOCS_AND_FREQS) {
    return allocator->allocate_object<Lucene90PostingsEnum<IndexOptions::DOCS_AND_FREQS>>(
        allocator,
        term_state,
        flags,
        DataInput<ReadOnlyDataInput>{doc_input.to_read_only()});
  } else if (index_options == IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
    if (flags <= PostingsEnum::Flags::FREQS) {
      return allocator->allocate_object<Lucene90PostingsEnum<IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>(
          allocator,
          term_state,
          flags,
          DataInput<ReadOnlyDataInput>{doc_input.to_read_only()});
    }

    // TODO
  } else if (index_options == IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS) {
    // TODO
  } // End if
}

ManagedPtr<ImpactsEnum> Lucene90PostingsReader::impacts(ManagedPtr<LinearAllocator> allocator,
                                                        ManagedPtr<FieldInfo> field_info,
                                                        const Lucene90TermState &term_state,
                                                        PostingsEnum::Flags flags) {
  if (term_state.doc_freq <= ForUtil::BLOCK_SIZE) {
    // No skip data
    return allocator->allocate_object<SlowImpactsEnum>(postings(allocator, field_info, term_state, flags));
  }

  const auto index_options = field_info->index_options;

  if (index_options == IndexOptions::DOCS) {
    return allocator->allocate_object<Lucene90ImpactsEnum<IndexOptions::DOCS>>(
        index_options,
        allocator,
        term_state,
        flags,
        DataInput<ReadOnlyDataInput>{doc_input.to_read_only()});
  } else if (index_options == IndexOptions::DOCS_AND_FREQS) {
    return allocator->allocate_object<Lucene90ImpactsEnum<IndexOptions::DOCS_AND_FREQS>>(
        index_options,
        allocator,
        term_state,
        flags,
        DataInput<ReadOnlyDataInput>{doc_input.to_read_only()});
  } else if (index_options == IndexOptions::DOCS_AND_FREQS_AND_POSITIONS) {
    return allocator->allocate_object<Lucene90ImpactsEnum<IndexOptions::DOCS_AND_FREQS_AND_POSITIONS>>(
        index_options,
        allocator,
        term_state,
        flags,
        DataInput<ReadOnlyDataInput>{doc_input.to_read_only()},
        DataInput<ReadOnlyDataInput>{pos_input.to_read_only()},
        DataInput<ReadOnlyDataInput>{pay_input.to_read_only()},
        false);
  }  // End if
}

Lucene90PostingsReaderPtr Lucene90PostingsReader::slice(
    ManagedPtr<MMapDirectory> directory,
    int64_t doc_fp,
    int64_t doc_map_size,
    int64_t pos_fp,
    int64_t pos_map_size,
    int64_t pay_fp,
    int64_t pay_map_size) {
  CloseableMemoryDataInput doc_input{};
  CloseableMemoryDataInput pos_input{};
  CloseableMemoryDataInput pay_input{};

  if (doc_fp > 0) {
    doc_input = directory->open_input_with_region(doc_file_name, doc_fp, doc_map_size);
  }
  if (pos_fp > 0) {
    pos_input = directory->open_input_with_region(pos_file_name, pos_fp, pos_map_size);
  }
  if (pay_fp > 0) {
    pay_input = directory->open_input_with_region(pay_file_name, pay_fp, pay_map_size);
  }

  return std::make_shared<Lucene90PostingsReader>(doc_file_name,
                                                  std::move(doc_input),
                                                  pos_file_name,
                                                  std::move(pos_input),
                                                  pay_file_name,
                                                  std::move(pay_input));
}
