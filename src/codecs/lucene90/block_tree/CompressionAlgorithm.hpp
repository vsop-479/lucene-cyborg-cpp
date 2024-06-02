#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_COMPRESSIONALGORITHM_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_COMPRESSIONALGORITHM_HPP_

#include "../../../util/PointerTypes.hpp"
#include "../../../store/DataInput.hpp"
#include "../../../store/ReadOnlyDataInput.hpp"

namespace lucene::cyborg::codecs::v90::block_tree {

enum class CompressionAlgorithmType {
  NO_COMPRESSION = 0, LOWERCASE_ASCII = 1, LZ4 = 2
};  // CompressionAlgorithmType

static constexpr int32_t NumSupportedCompressionAlgorithms = 3;

class CompressionAlgorithm {
 public:
  static util::ManagedPtr<CompressionAlgorithm> get_algorithm(uint32_t code);

  explicit CompressionAlgorithm(CompressionAlgorithmType _compression_algorithm_type)
      : compression_algorithm_type(_compression_algorithm_type) {
  }

  virtual ~CompressionAlgorithm() = default;

  virtual void read(store::DataInput<store::ReadOnlyDataInput> &in, util::OverwriteBytes out, int32_t len) = 0;

  CompressionAlgorithmType compression_algorithm_type;
};  // CompressionAlgorithm



}  // lucene::cyborg::codecs::v90::block_tree

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_COMPRESSIONALGORITHM_HPP_
