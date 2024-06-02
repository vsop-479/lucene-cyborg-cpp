#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_MMAPDIRECTORYREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_MMAPDIRECTORYREADER_HPP_

#include <memory>
#include "../store/MMapDirectory.hpp"
#include "../util/Version.hpp"
#include "SegmentInfos.hpp"
#include "SegmentReader.hpp"
#include "LeafReaderContext.hpp"

namespace lucene::cyborg::index {

class MMapDirectoryReader;
using MMapDirectoryReaderPtr = std::shared_ptr<MMapDirectoryReader>;

class MMapDirectoryReader {
 public:
  static MMapDirectoryReaderPtr open(
      store::MMapDirectoryPtr &directory,
      int32_t min_supported_major_version = util::Version::MIN_SUPPORTED_MAJOR);

  MMapDirectoryReader(store::MMapDirectoryPtr _directory,
                      std::vector<SegmentReaderPtr> _sub_readers,
                      SegmentInfosPtr _sis);

  std::vector<int32_t> starts;
  std::vector<SegmentReaderPtr> sub_readers;
  std::vector<SegmentReaderPtr> sub_readers_list_preserve;
  std::vector<LeafReaderContext> leaves;
  SegmentInfosPtr segment_infos;
  store::MMapDirectoryPtr directory;
  int32_t max_doc;
};  // MMapDirectoryReader



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_MMAPDIRECTORYREADER_HPP_
