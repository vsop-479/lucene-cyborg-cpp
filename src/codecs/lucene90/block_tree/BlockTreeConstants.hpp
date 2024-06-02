#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_BLOCKTREECONSTANTS_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_BLOCKTREECONSTANTS_HPP_

#include <cstdint>

namespace lucene::cyborg::codecs::v90::block_tree {

struct BlockTreeConstants {
  static constexpr uint32_t OUTPUT_FLAGS_NUM_BITS = 2U;
  static constexpr uint32_t OUTPUT_FLAG_HAS_TERMS = 0x2U;
  static constexpr uint32_t OUTPUT_FLAG_IS_FLOOR = 0x1U;
};  // BlockTreeConstants



}  // lucene::cyborg::codecs::v90::block_tree

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_BLOCKTREECONSTANTS_HPP_
