#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADSTATE_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADSTATE_HPP_

#include "SegmentInfo.hpp"
#include "../store/MMapDirectory.hpp"
#include "FieldInfos.hpp"

namespace lucene::cyborg::index {

struct SegmentReadState {
  /** {@link Directory} where this segment is read from. */
  store::MMapDirectoryPtr directory;

  /** {@link SegmentInfo} describing this segment. */
  SegmentInfoPtr segment_info;

  /** {@link FieldInfos} describing all fields fst_in this segment. */
  FieldInfosPtr field_infos;

  /**
   * Unique suffix for any postings files read for this segment. {@link PerFieldPostingsFormat} sets
   * this for each of the postings formats it wraps. If you create a new {@link PostingsFormat} then
   * any files you write/read must be derived using this suffix (use {@link
   * IndexFileNames#segmentFileName(String,String,String)}).
   */
  std::string segment_suffix;
};  // SegmentReadState



};  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADSTATE_HPP_
