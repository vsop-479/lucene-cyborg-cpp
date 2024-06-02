#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_LUCENE90BLOCKTREETERMSREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_LUCENE90BLOCKTREETERMSREADER_HPP_

#include <memory>
#include "../../../search/SearchContext.hpp"
#include "../../FieldsProducer.hpp"
#include "../Lucene90PostingsReader.hpp"
#include "../../../store/DataInput.hpp"
#include "../../../store/BufferedChecksumMemoryDataInput.hpp"
#include "FieldReader.hpp"

namespace lucene::cyborg::codecs::v90::block_tree {

class Lucene90BlockTreeTermsReader final : public FieldsProducer {
 public:
  Lucene90BlockTreeTermsReader(Lucene90PostingsReaderPtr postings_reader,
                               util::ReadOnlyPtr<index::SegmentReadState> state);

  void close() final;

  void check_integrity() final;

  int32_t size() final;

  util::ManagedPtr<FieldReader> terms(
      std::string_view field_name,
      util::ManagedPtr<search::SearchContext> search_context) final;

  // Reads the terms dict entries, to gather state to
  // produce DocsEnum on demand
  Lucene90PostingsReaderPtr postings_reader;
  std::unordered_map<std::string_view, FieldReaderPtr> field_map;
  std::unique_ptr<char[]> field_name_bytes;
  std::vector<std::string> field_list;
  std::string segment;
  int32_t version;
};  // Lucene90BlockTreeTermsReader

using Lucene90BlockTreeTermsReaderPtr = std::shared_ptr<Lucene90BlockTreeTermsReader>;

}  // lucene::cyborg::codecs::v90::block_tree

#endif // LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_LUCENE90BLOCKTREETERMSREADER_HPP_
