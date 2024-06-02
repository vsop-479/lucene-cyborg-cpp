#include "Lucene90NormsProducer.hpp"
#include "Lucene90NormsFormat.hpp"
#include "../CodecUtil.hpp"
#include "../../index/DocValues.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::v90::IndexedDISI;
using lucene::cyborg::codecs::v90::Lucene90DenseAllEqualsNormsIterator;
using lucene::cyborg::codecs::v90::Lucene90DenseByteNormsIterator;
using lucene::cyborg::codecs::v90::Lucene90DenseNormsIterator;
using lucene::cyborg::codecs::v90::Lucene90NormsEntry;
using lucene::cyborg::codecs::v90::Lucene90NormsFormat;
using lucene::cyborg::codecs::v90::Lucene90NormsProducer;
using lucene::cyborg::index::DocValues;
using lucene::cyborg::index::FieldInfo;
using lucene::cyborg::index::FieldInfos;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::NumericDocValues;
using lucene::cyborg::index::SegmentReadState;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// Lucene90NormsProducer
//

Lucene90NormsProducer::Lucene90NormsProducer(ReadOnlyPtr<SegmentReadState> _state)
    : norms(),
      max_doc(_state->segment_info->max_doc),
      data(),
      disi_inputs(),
      disi_jump_tables(),
      data_inputs() {
  const std::string meta_name =
      IndexFileNames::segment_file_name(_state->segment_info->name,
                                        _state->segment_suffix,
                                        std::string(Lucene90NormsFormat::METADATA_EXTENSION));
  int32_t version = -1;

  // Read in the entries from the metadata file.
  {
    auto meta_input = _state->directory->open_input(meta_name);
    auto meta_read_only_input = meta_input.to_read_only();
    DataInput<BufferedChecksumMemoryDataInput> meta_checksum_in{
        BufferedChecksumMemoryDataInput{
            meta_read_only_input.data,
            meta_read_only_input.offset,
            meta_read_only_input.length,
            BufferedChecksum{}
        }
    };

    version = CodecUtil::check_index_header(meta_checksum_in,
                                            std::string(Lucene90NormsFormat::METADATA_CODEC),
                                            Lucene90NormsFormat::VERSION_START,
                                            Lucene90NormsFormat::VERSION_CURRENT,
                                            _state->segment_info->id,
                                            _state->segment_suffix);

    read_fields(meta_checksum_in, _state->field_infos.get());
  }

  const std::string data_name =
      IndexFileNames::segment_file_name(_state->segment_info->name,
                                        _state->segment_suffix,
                                        std::string(Lucene90NormsFormat::DATA_EXTENSION));
  data = _state->directory->open_input(data_name);
  {
    auto data_read_only_input = data.to_read_only();
    DataInput<BufferedChecksumMemoryDataInput> data_checksum_in{
        BufferedChecksumMemoryDataInput{
            data_read_only_input.data,
            data_read_only_input.offset,
            data_read_only_input.length,
            BufferedChecksum{}
        }
    };

    const int32_t version2 = CodecUtil::check_index_header(
        data_checksum_in,
        std::string(Lucene90NormsFormat::DATA_CODEC),
        Lucene90NormsFormat::VERSION_START,
        Lucene90NormsFormat::VERSION_CURRENT,
        _state->segment_info->id,
        _state->segment_suffix);

    if (version2 != version) {
      // TODO
      throw 13;
      //  throw new CorruptIndexException(
      //      "Format versions mismatch: meta=" + version + ",data=" + version2, data);
    }

    // NOTE: data file is too costly to verify checksum against all the bytes on open,
    // but for now we at least verify proper structure of the checksum footer: which looks
    // for FOOTER_MAGIC + algorithmID. This is cheap and can detect some forms of corruption
    // such as file truncation.
    CodecUtil::retrieve_checksum(data_checksum_in);
  }
}

DataInput<ReadOnlyDataInput> get_data_input(CloseableMemoryDataInput &data, const Lucene90NormsEntry &entry) {
  return DataInput<ReadOnlyDataInput>{
      data.slice(entry.norms_offset, entry.num_docs_with_field + (int64_t) entry.bytes_per_norm)
  };
}

ManagedPtr<NumericDocValues> Lucene90NormsProducer::get_norms(ManagedPtr<LinearAllocator> allocator,
                                                              UseOnlyPtr<FieldInfo> field) {
  if (field->number < norms.size()) {
    auto &entry_opt = norms[field->number];
    if (entry_opt) {
      const auto &entry = *entry_opt;

      if (entry.docs_with_field_offset == -2) {
        // empty
        return DocValues::empty_numeric(allocator);
      } else if (entry.docs_with_field_offset == -1) {
        // dense
        if (entry.bytes_per_norm == 0) {
          return allocator->allocate_object<Lucene90DenseAllEqualsNormsIterator>(entry.norms_offset, max_doc);
        }

        auto slice = get_data_input(data, entry);

        switch (entry.bytes_per_norm) {
          case 1:return allocator->allocate_object<Lucene90DenseByteNormsIterator<1>>(max_doc, slice);
          case 2:return allocator->allocate_object<Lucene90DenseByteNormsIterator<2>>(max_doc, slice);
          case 4:return allocator->allocate_object<Lucene90DenseByteNormsIterator<4>>(max_doc, slice);
          case 8:return allocator->allocate_object<Lucene90DenseByteNormsIterator<8>>(max_doc, slice);
          default:
            // should not happen, we already validate bytesPerNorm in readFields
            // throw new AssertionError();
            // TODO
            throw 13;
        }
      } else {
        // sparse
        if (entry.bytes_per_norm == 0) {
          return allocator->allocate_object<Lucene90SparseAllEqualNormsIterator>(
              IndexedDISI{allocator,
                          IndexedDISI::create_block_slice(data, entry),
                          IndexedDISI::create_jump_table(data, entry),
                          entry},
              entry.norms_offset);
        }

        switch (entry.bytes_per_norm) {
          case 1:
            return allocator->allocate_object < Lucene90SparseByteNormsIterator < 1 >> (
                IndexedDISI{allocator,
                            IndexedDISI::create_block_slice(data, entry),
                            IndexedDISI::create_jump_table(data, entry),
                            entry},
                    get_data_input(data, entry));
          case 2:
            return allocator->allocate_object < Lucene90SparseByteNormsIterator < 2 >> (
                IndexedDISI{allocator,
                            IndexedDISI::create_block_slice(data, entry),
                            IndexedDISI::create_jump_table(data, entry),
                            entry},
                    get_data_input(data, entry));
          case 4:
            return allocator->allocate_object < Lucene90SparseByteNormsIterator < 4 >> (
                IndexedDISI{allocator,
                            IndexedDISI::create_block_slice(data, entry),
                            IndexedDISI::create_jump_table(data, entry),
                            entry},
                    get_data_input(data, entry));
          case 8:
            return allocator->allocate_object < Lucene90SparseByteNormsIterator < 8 >> (
                IndexedDISI{allocator,
                            IndexedDISI::create_block_slice(data, entry),
                            IndexedDISI::create_jump_table(data, entry),
                            entry},
                    get_data_input(data, entry));
          default:
            // should not happen, we already validate bytesPerNorm in readFields
            // throw new AssertionError();
            // TODO
            throw 13;
        }  // End switch
      }  // End if
    }  // End if
  }  // End if

  // TODO : Field Not found
  throw 13;
}

void Lucene90NormsProducer::close() {
  data.close();
}

void Lucene90NormsProducer::check_integrity() {
  CodecUtil::checksum_entire_file(DataInput<ReadOnlyDataInput>{
      data.to_read_only()
  });
}

void Lucene90NormsProducer::read_fields(DataInput<BufferedChecksumMemoryDataInput> &meta,
                                        UseOnlyPtr<FieldInfos> field_infos) {
  int32_t max_field_number = 0;
  for (int32_t field_number = meta.read_int(); field_number != -1; field_number = meta.read_int()) {
    auto info = field_infos->field_info(field_number);
    if (info == nullptr) {
      // TODO
      throw 13;
      // throw new CorruptIndexException("Invalid field number: " + fieldNumber, meta);
    } else if (!info->has_norms()) {
      // TODO
      throw 13;
      // throw new CorruptIndexException("Invalid field: " + info.name, meta);
    }  // End if

    Lucene90NormsEntry entry;
    entry.docs_with_field_offset = meta.read_long();
    entry.docs_with_field_length = meta.read_long();
    entry.jump_table_entry_count = meta.read_short();
    const int32_t dense_rank_power = meta.read_byte();
    if (dense_rank_power != -1) {
      entry.dense_rank_power = dense_rank_power;
    } else {
      entry.dense_rank_power = 127U;
    }
    entry.num_docs_with_field = meta.read_int();
    entry.bytes_per_norm = meta.read_byte();
    switch (entry.bytes_per_norm) {
      case 0:
      case 1:
      case 2:
      case 4:
      case 8:break;
      default:
        // TODO
        throw 13;
        // throw new CorruptIndexException(
        //     "Invalid bytesPerValue: " + entry.bytesPerNorm + ", field: " + info.name, meta);
    }  // End switch

    entry.norms_offset = meta.read_long();

    if (norms.size() <= info->number) {
      norms.resize(info->number * 2);
    }
    norms[info->number] = entry;
    if (info->number > max_field_number) {
      max_field_number = info->number;
    }
  }  // End for

  norms.resize(max_field_number + 1);
  norms.shrink_to_fit();
}

Lucene90NormsProducer::~Lucene90NormsProducer() {
  close();
}
