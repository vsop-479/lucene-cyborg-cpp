#include "CompressionAlgorithm.hpp"

using lucene::cyborg::codecs::v90::block_tree::CompressionAlgorithm;
using lucene::cyborg::codecs::v90::block_tree::CompressionAlgorithmType;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MemoryUtils;
using lucene::cyborg::util::OverwriteBytes;
using lucene::cyborg::util::ReadOnlyPtr;

//
// NoCompression
//

struct NoCompression final : public CompressionAlgorithm {
  NoCompression()
      : CompressionAlgorithm(CompressionAlgorithmType::NO_COMPRESSION) {
  }

  void read(DataInput<ReadOnlyDataInput> &in, OverwriteBytes out, int32_t len) final {
    MemoryUtils::memcpy(out, in.input.data, len);
    in.skip_bytes(len);
  }
};  // NoCompression

struct LowercaseAscii final : public CompressionAlgorithm {
  LowercaseAscii()
      : CompressionAlgorithm(CompressionAlgorithmType::LOWERCASE_ASCII) {
  }

  void read(DataInput<ReadOnlyDataInput> &in, OverwriteBytes out, int32_t len) final {
    const int32_t saved = ((uint32_t) len) >> 2;
    int32_t compressed_len = len - saved;

    // 1. Copy the packed bytes
    MemoryUtils::memcpy(out, in.input.data, compressed_len);
    in.skip_bytes(compressed_len);

    // 2. Restore the leading 2 bits of each packed byte into whole bytes
    for (int32_t i = 0; i < saved; ++i) {
      const uint8_t b0 = (out[i] & 0xC0U) >> 2U;
      const uint8_t b1 = (out[saved + i] & 0xC0U) >> 4U;
      const uint8_t b2 = (out[(saved << 1U) + i] & 0xC0U) >> 6U;
      out[compressed_len + i] = b0 | b1 | b2;
    }

    // 3. Move back to the original range.
    for (int32_t i = 0; i < len; ++i) {
      const uint8_t b = out[i];
      const auto b0 = b & 0x1FU;
      const auto b1 = ((b & 0x20U) << 1U);
      out[i] = (b0 | 0x20U | b1) - 1;
    }

    // 4. Restore exceptions
    const int32_t num_exceptions = in.read_vint();
    for (int32_t exception = 0, i = 0; exception < num_exceptions; ++exception) {
      i += in.read_byte();
      out[i] = in.read_byte();
    }
  }
};  // LowercaseAscii

struct Lz4 final : public CompressionAlgorithm {
  Lz4()
      : CompressionAlgorithm(CompressionAlgorithmType::LZ4) {
  }

  void read(DataInput<ReadOnlyDataInput> &in, OverwriteBytes out, const int32_t len) final {
    int32_t doff = 0;
    do {
      const auto token = in.read_byte();
      auto literal_len = token >> 4;

      if (literal_len != 0) {
        if (literal_len == 0x0FU) {
          uint32_t l;
          while ((l = in.read_byte()) == 0xFFU) {
            literal_len += 0xFFU;
          }
          literal_len += l & 0xFFU;
        }
        MemoryUtils::memcpy(out + doff, in.input.data, literal_len);
        in.skip_bytes(literal_len);
        doff += literal_len;
      }  // End if

      if (doff >= len) {
        return;
      }

      // matches
      const auto match_dec = in.read_short() & 0xFFFFU;
      assert(((int32_t) match_dec) > 0);
      auto match_len = token & 0x0FU;
      if (match_len == 0x0FU) {
        uint32_t l;
        while ((l = in.read_byte()) == 0xFFU) {
          match_len += 0xFFU;
        }
        match_len += l & 0xFFU;
      }  // End if
      match_len += 4;  // MIN_MATCH = 4

      if (match_dec < match_len) {
        // overlap -> naive incremental copy
        for (auto ref = doff - match_dec, end = doff + match_len; doff < end; ++ref, ++doff) {
          out[doff] = out[ref];
        }
      } else {
        // no overlap -> memcpy
        MemoryUtils::memcpy(out + doff, out + (doff - match_dec), match_len);
        doff += match_len;
      }  // End if
    } while (doff < len);
  }
};  // Lz4

//
//  CompressionAlgorithm
//

ManagedPtr<CompressionAlgorithm> CompressionAlgorithm::get_algorithm(uint32_t code) {
  static NoCompression NO_COMPRESSION{};
  static LowercaseAscii LOWERCASE_ASCII{};
  static Lz4 LZ4{};

  if (code == 0) {
    return &NO_COMPRESSION;
  } else if (code == 1) {
    return &LOWERCASE_ASCII;
  } else if (code == 2) {
    return &LZ4;
  } else {
    return nullptr;
  }
}
