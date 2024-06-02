#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_CODECUTIL_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_CODECUTIL_HPP_

#include <cstdint>
#include <string>
#include "../util/PointerTypes.hpp"
#include "../store/DataInput.hpp"
#include "../store/BufferedChecksumMemoryDataInput.hpp"

namespace lucene::cyborg::codecs {

class CodecUtil {
 public:
  // Constant to identify the start of a codec header.
  static constexpr int32_t CODEC_MAGIC = 0x3fd76c17;

  // Constant to identify the start of a codec footer.
  static constexpr int32_t FOOTER_MAGIC = ~CODEC_MAGIC;

  template<typename DataInput>
  static int32_t check_header_no_magic(
      DataInput &input,
      const std::string &codec,
      int32_t min_version,
      int32_t max_version) {
    const auto actual_codec_str_len = input.read_vint();
    if (actual_codec_str_len != ((int32_t) codec.size())) {
      throw 13;
    }
    std::string actual_codec;
    actual_codec.resize(actual_codec_str_len);
    input.read_bytes(actual_codec.data(), actual_codec_str_len);
    if (actual_codec != codec) {
      // TODO
      throw 13;
    }

    const int32_t actual_version = input.read_be_int();
    if (actual_version < min_version) {
      // TODO
      throw 13;
    }
    if (actual_version > max_version) {
      // TODO
      throw 13;
    }

    return actual_version;
  }

  static int64_t checksum_entire_file(store::DataInput<store::ReadOnlyDataInput> input);

  static int64_t estimate_header_size(const int32_t codec_str_len) noexcept;

  static std::string check_index_header_suffix(
      store::DataInput<store::BufferedChecksumMemoryDataInput> &input,
      const std::string &expected_suffix);

  static int64_t check_footer(store::DataInput<store::BufferedChecksumMemoryDataInput> &in);

  static std::string check_index_header_id(store::DataInput<store::BufferedChecksumMemoryDataInput> &in,
                                           const std::string &expected_id);

  template<typename DataInput>
  static int32_t check_header(DataInput &in,
                              const std::string &codec,
                              int32_t min_version,
                              int32_t max_version) {
    const int32_t actual_header = in.read_be_int();
    if (actual_header != CODEC_MAGIC) {
      // TODO
      throw 13;
    }
    return check_header_no_magic(in, codec, min_version, max_version);
  }

  static void validate_footer(store::DataInput<store::BufferedChecksumMemoryDataInput> &in);

  static int32_t check_index_header(
      store::DataInput<store::BufferedChecksumMemoryDataInput> &in,
      const std::string &codec,
      int32_t min_version,
      int32_t max_version,
      const std::string &expected_id,
      const std::string &expected_suffix);

  static int64_t retrieve_checksum(
      store::DataInput<store::BufferedChecksumMemoryDataInput> &in);

  static int64_t read_crc(store::DataInput<store::BufferedChecksumMemoryDataInput> &in);

  static int32_t maximum_size_for_checking_index_header(const std::string &codec);

  static constexpr int32_t footer_length() noexcept {
    return 16;
  }
};  // CodecUtil



}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_CODECUTIL_HPP_
