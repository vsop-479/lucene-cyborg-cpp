#include "CodecUtil.hpp"
#include "../util/StringHelper.hpp"
#include "../util/BufferedChecksum.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::StringHelper;

//
// CodecUtil
//

std::string CodecUtil::check_index_header_suffix(DataInput<BufferedChecksumMemoryDataInput> &input,
                                                 const std::string &expected_suffix) {
  const int32_t suffix_length = input.read_byte();
  std::string suffix(suffix_length, '\0');
  input.read_bytes(suffix.data(), suffix_length);
  if (suffix != expected_suffix) {
    // TODO
    throw 13;
  }
  return suffix;
}

int64_t CodecUtil::check_footer(DataInput<BufferedChecksumMemoryDataInput> &in) {
  validate_footer(in);
  const int64_t actual_checksum = in.input.buffered_checksum.get_value();
  const int64_t expected_checksum = read_crc(in);
  if (actual_checksum != expected_checksum) {
    // TODO
    throw 13;
  }
  return actual_checksum;
}

int32_t CodecUtil::check_index_header(DataInput<BufferedChecksumMemoryDataInput> &in,
                                      const std::string &codec,
                                      int32_t min_version,
                                      int32_t max_version,
                                      const std::string &expected_id,
                                      const std::string &expected_suffix) {
  const int32_t version = check_header(in, codec, min_version, max_version);
  check_index_header_id(in, expected_id);
  check_index_header_suffix(in, expected_suffix);
  return version;
}

std::string CodecUtil::check_index_header_id(DataInput<BufferedChecksumMemoryDataInput> &in,
                                             const std::string &expected_id) {
  std::string id(StringHelper::ID_LENGTH, '\0');
  in.read_bytes(id.data(), StringHelper::ID_LENGTH);
  if (id != expected_id) {
    // TODO
    throw 13;
  }
  return expected_id;
}

void CodecUtil::validate_footer(DataInput<BufferedChecksumMemoryDataInput> &in) {
  const int64_t remaining = in.input.actual_length() - in.get_file_pointer();
  const int64_t expected = footer_length();
  if (remaining < expected) {
    // TODO
    throw 13;
  } else if (remaining > expected) {
    // TODO
    throw 13;
  }

  const int32_t magic = in.read_be_int();
  if (magic != FOOTER_MAGIC) {
    // TODO
    throw 13;
  }

  const int32_t algorithm_id = in.read_be_int();
  if (algorithm_id != 0) {
    // TODO
    throw 13;
  }
}

int64_t CodecUtil::read_crc(DataInput<BufferedChecksumMemoryDataInput> &in) {
  const uint64_t value = in.read_be_long();
  if (value & 0xFFFFFFFF00000000UL) {
    // TODO
    throw 13;
  }
  return value;
}

int32_t CodecUtil::maximum_size_for_checking_index_header(const std::string &codec) {
  return 4                       // read be int
      + 4                        // read be int
      + codec.size()             // read codec
      + StringHelper::ID_LENGTH  // read id
      + 128;                     // read suffix bytes
}

int64_t CodecUtil::retrieve_checksum(DataInput<BufferedChecksumMemoryDataInput> &in) {
  if (in.input.actual_length() < footer_length()) [[unlikely]] {
    // TODO
    throw 13;
  }
  in.seek(in.input.actual_length() - footer_length());
  validate_footer(in);
  return read_crc(in);
}

int64_t CodecUtil::estimate_header_size(const int32_t codec_str_len) noexcept {
  return 4                       // read_be_int
      + codec_str_len            // read_string
      + 4                        // read_be_int
      + StringHelper::ID_LENGTH  // read_bytes
      + 1                        // read_byte
      + 255;                     // read_bytes
}

int64_t CodecUtil::checksum_entire_file(DataInput<ReadOnlyDataInput> input) {
  input.seek(0);
  const int64_t remaining = input.input.length - input.get_file_pointer();
  if (remaining < footer_length()) {
    // TODO
    throw 13;
    // throw new CorruptIndexException(
    //     "misplaced codec footer (file truncated?): length="
    //         + in.length()
    //         + " but footerLength=="
    //         + footerLength(),
    //     input);
  }

  DataInput<BufferedChecksumMemoryDataInput> checksum_in{
      BufferedChecksumMemoryDataInput{
          input.input.data,
          input.input.offset,
          input.input.length,
          BufferedChecksum{}
      }
  };

  assert(checksum_in.get_file_pointer() == 0);

  checksum_in.seek(input.input.length - footer_length());
  return check_footer(checksum_in);
}
