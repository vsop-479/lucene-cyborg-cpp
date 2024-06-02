#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSREADER_HPP_

#include "../PostingsReaderBase.hpp"
#include "../../index/SegmentReadState.hpp"
#include "../../store/DataInput.hpp"
#include "../../store/BufferedChecksumMemoryDataInput.hpp"
#include "../../index/FieldInfo.hpp"
#include "../../index/PostingsEnum.hpp"
#include "../../util/LinearAllocator.hpp"
#include "DocPosPayFilePointers.hpp"
#include "Lucene90TermState.hpp"

namespace lucene::cyborg::codecs::v90 {

class Lucene90PostingsReader;
using Lucene90PostingsReaderPtr = std::shared_ptr<Lucene90PostingsReader>;

class Lucene90PostingsReader : public PostingsReaderBase {
 public:
  using TermState = Lucene90TermState;

  explicit Lucene90PostingsReader(util::ReadOnlyPtr<index::SegmentReadState> state);

  Lucene90PostingsReader(const std::string &doc_file_name,
                         store::CloseableMemoryDataInput doc_input,
                         const std::string &pos_file_name,
                         store::CloseableMemoryDataInput pos_input,
                         const std::string &pay_file_name,
                         store::CloseableMemoryDataInput pay_input);

  void format_term_state(util::ManagedPtr<index::FieldInfo> field_info, Lucene90TermState &term_state);

  void init(store::DataInput<store::BufferedChecksumMemoryDataInput> &terms_in,
            util::ReadOnlyPtr<index::SegmentReadState> state);

  void decode_term(store::DataInput<store::ReadOnlyDataInput> &in,
                   Lucene90TermState &term_state);

  util::ManagedPtr<index::PostingsEnum> postings(util::ManagedPtr<util::LinearAllocator> allocator,
                                                 util::ManagedPtr<index::FieldInfo> field_info,
                                                 const Lucene90TermState &term_state,
                                                 index::PostingsEnum::Flags flags);

  util::ManagedPtr<index::ImpactsEnum> impacts(util::ManagedPtr<util::LinearAllocator> allocator,
                                               util::ManagedPtr<index::FieldInfo> field_info,
                                               const Lucene90TermState &term_state,
                                               index::PostingsEnum::Flags flags);

  Lucene90PostingsReaderPtr slice(util::ManagedPtr<store::MMapDirectory> directory,
                                  int64_t doc_fp, int64_t doc_map_size,
                                  int64_t pos_fp, int64_t pos_map_size,
                                  int64_t pay_fp, int64_t pay_map_size);

  int64_t get_doc_file_size() const noexcept {
    return doc_file_size;
  }

  int64_t get_pos_file_size() const noexcept {
    return pos_file_size;
  }

  int64_t get_pay_file_size() const noexcept {
    return pay_file_size;
  }

 private:
  int64_t doc_file_size;
  int64_t pos_file_size;
  int64_t pay_file_size;
  std::string doc_file_name;
  std::string pos_file_name;
  std::string pay_file_name;
  store::CloseableMemoryDataInput doc_input;
  store::CloseableMemoryDataInput pos_input;
  store::CloseableMemoryDataInput pay_input;
};  // Lucene90PostingsReader

}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90POSTINGSREADER_HPP_
