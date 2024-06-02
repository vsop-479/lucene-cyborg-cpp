#include "FstBytesOutput.hpp"

using lucene::cyborg::util::FstReverseBytes;
using lucene::cyborg::util::FstBytesOutput;

//
// FstBytesOutput
//

const FstReverseBytes &FstBytesOutput::get_no_output() const noexcept {
  static FstReverseBytes NO_OUTPUT{nullptr, 0};
  return NO_OUTPUT;
}

void FstBytesOutput::skip_output(FstReverseBytesReader &reader) const {
  if (const auto len = reader.read_vint(); len != 0) {
    reader.skip_bytes(len);
  }
}

void FstBytesOutput::skip_final_output(FstReverseBytesReader &reader) const {
  skip_output(reader);
}

void FstBytesOutput::read_output(FstReverseBytesReader &reader,
                                 FstReverseBytes &bytes) const {
  const auto len = reader.read_vint();
  bytes.len = len;
  bytes.bytes = reader.data;
  reader.skip_bytes(len);
}

void FstBytesOutput::read_final_output(FstReverseBytesReader &reader,
                                       FstReverseBytes &bytes) const {
  read_output(reader, bytes);
}
