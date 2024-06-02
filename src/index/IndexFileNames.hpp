#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_INDEXFILENAMES_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_INDEXFILENAMES_HPP_

#include <string>
#include <cstdint>

namespace lucene::cyborg::index {

class IndexFileNames final {
 public:
  static const std::string SEGMENTS;

  static std::string segment_file_name(const std::string &segment_name,
                                       const std::string &segment_suffix,
                                       const std::string &ext);

  static std::string file_name_from_generation(const std::string &base, const std::string &ext, int64_t gen);

  static std::string strip_segment_name(const std::string &file_name);

  static int32_t index_of_segment_name(const std::string &file_name);
};  // IndexFileNames



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_INDEXFILENAMES_HPP_
