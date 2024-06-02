#include <limits>
#include "MMapDirectoryReader.hpp"
#include "../index/FindSegmentsFile.hpp"
#include "../index/IndexWriterConstants.hpp"
#include "../util/IoUtils.hpp"

using lucene::cyborg::index::FindSegmentsFile;
using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::index::MMapDirectoryReaderPtr;
using lucene::cyborg::index::SegmentInfos;
using lucene::cyborg::index::SegmentInfosPtr;
using lucene::cyborg::index::IndexWriterConstants;
using lucene::cyborg::index::SegmentReader;
using lucene::cyborg::index::SegmentReaderPtr;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::IOUtils;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::Version;

struct DirectoryReaderFindSegmentFiles final : public FindSegmentsFile<MMapDirectoryReaderPtr> {
  int32_t min_supported_major_version;

  DirectoryReaderFindSegmentFiles(MMapDirectoryPtr _directory, int32_t _min_supported_major_version)
      : FindSegmentsFile(std::move(_directory)),
        min_supported_major_version(_min_supported_major_version) {
  }

  MMapDirectoryReaderPtr doBody(const std::string &segment_file_name) final {
    if (min_supported_major_version > Version::LATEST->major || min_supported_major_version < 0) {
      // TODOe
      // throw new IllegalArgumentException(
      //     "minSupportedMajorVersion must be positive and <= "
      //         + Version.LATEST.major
      //         + " but was: "
      //         + minSupportedMajorVersion);
      throw 13;
    }

    auto sis = SegmentInfos::read_commit(directory, segment_file_name, min_supported_major_version);
    std::vector<SegmentReaderPtr> readers(sis->size());
    try {
      for (int32_t i = sis->size() - 1; i >= 0; --i) {
        // TODO : pass the segment codec name to subreader.
        //        we can get the segment codec name via sis->get_commit_infos(i)->info->codec_name
        readers[i] =
            std::make_shared<SegmentReader>(sis->info(i), sis->get_index_created_version_major());
      }
      // This may throw CorruptIndexException if there are too many docs,
      // so it must be inside try clause, so we close readers fst_in that case:
      auto reader =
          std::make_shared<MMapDirectoryReader>(directory, std::move(readers), std::move(sis));

      return reader;
    } catch (...) {
      // TODO : Do we need this?
      // IOUtils::close_while_handling_exception(readers);
      throw 13;
    }
  }
};  // DirectoryReaderFindSegmentFiles


//
// MMapDirectoryReader
//

MMapDirectoryReaderPtr MMapDirectoryReader::open(MMapDirectoryPtr &directory,
                                                 int32_t min_supported_major_version) {
  DirectoryReaderFindSegmentFiles factory(directory, min_supported_major_version);
  return factory.run();
}

MMapDirectoryReader::MMapDirectoryReader(MMapDirectoryPtr _directory,
                                         std::vector<SegmentReaderPtr> _sub_readers,
                                         SegmentInfosPtr _sis)
    : starts(_sub_readers.size() + 1),
      sub_readers(_sub_readers),
      sub_readers_list_preserve(_sub_readers),
      leaves(),
      segment_infos(),
      directory(),
      max_doc() {
  int64_t max_doc = 0;
  for (int32_t i = 0; i < _sub_readers.size(); ++i) {
    starts[i] = (int32_t) max_doc;
    auto &r = _sub_readers[i];
    max_doc += r->get_max_doc(); // compute max_docs
    // TODO
    // r.register_parent_reader(this);
  }

  if (max_doc > IndexWriterConstants::ACTUAL_MAX_DOCS) {
    // TODO
    // A single index has too many documents and it is corrupt (IndexWriterConstants prevents this as of
    // LUCENE-6299)
    // throw new CorruptIndexException(
    //     "Too many documents: an index cannot exceed "
    //         + IndexWriterConstants.getActualMaxDocs()
    //         + " but readers have total maxDoc="
    //         + maxDoc,
    //     Arrays.toString(subReaders));
    throw 13;
  }  // End if

  if (max_doc > std::numeric_limits<int32_t>::max()
      || max_doc < std::numeric_limits<int32_t>::min()) {
    // TODO
    throw 13;
  }

  this->max_doc = max_doc;
  starts.back() = this->max_doc;

  leaves.reserve(sub_readers.size());
  int32_t doc_base = 0;
  for (int32_t i = 0; i < sub_readers.size(); ++i) {
    leaves.emplace_back(LeafReaderContext{i, doc_base, sub_readers[i].get()});
    doc_base += sub_readers[i]->get_max_doc();
  }
  assert(doc_base == max_doc);
}
