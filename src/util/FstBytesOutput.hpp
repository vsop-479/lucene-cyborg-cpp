#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_FSTBYTESOUTPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_FSTBYTESOUTPUT_HPP_

#include "PointerTypes.hpp"
#include "FstReverseBytesReader.hpp"

namespace lucene::cyborg::util {

struct FstReverseBytes {
  ReadOnlyPtr<uint8_t> bytes;
  int32_t len;
  auto operator<=>(const FstReverseBytes &) const = default;
};  // FstReverseBytes



struct FstBytesOutput {
  [[nodiscard]] const FstReverseBytes &get_no_output() const noexcept;

  void skip_output(FstReverseBytesReader &reader) const;

  void skip_final_output(FstReverseBytesReader &reader) const;

  void read_output(FstReverseBytesReader &reader, FstReverseBytes &bytes) const;

  void read_final_output(FstReverseBytesReader &reader, FstReverseBytes &bytes) const;
};  // FstBytesOutput



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_FSTBYTESOUTPUT_HPP_
