#include "SegmentInfo.hpp"
#include "../index/IndexFileNames.hpp"
#include "../util/StringHelper.hpp"

using lucene::cyborg::index::SegmentInfo;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::Version;
using lucene::cyborg::util::StringHelper;

//
// SegmentInfo
//

SegmentInfo::SegmentInfo(MMapDirectoryPtr _dir,
                         const Version &_version,
                         const Version &_min_version,
                         const std::string &_name,
                         int32_t _max_doc,
                         bool _is_compound_file,
                         const std::string &_codec_name,
                         std::unordered_map<std::string, std::string> _diagnostics,
                         const std::string &_id,
                         std::unordered_map<std::string, std::string> _attributes)
    : name(_name),
      max_doc(_max_doc),
      dir(std::move(_dir)),
      is_compound_file(_is_compound_file),
      id(_id),
      diagnostics(std::move(_diagnostics)),
      attributes(std::move(_attributes)),
      version(_version),
      min_version(_min_version),
      codec_name(_codec_name),
      files() {
  if (_id.length() != StringHelper::ID_LENGTH) [[unlikely]] {
    // TODO
    throw 13;
  }
}

std::string SegmentInfo::named_for_this_segment(const std::string &file) const {
  return name + IndexFileNames::strip_segment_name(file);
}

void check_file_names(const std::unordered_set<std::string> &files) {
  // TODO
}

void SegmentInfo::set_files(const std::unordered_set<std::string> &files) {
  this->files.clear();
  check_file_names(files);
  for (auto &file : files) {
    this->files.emplace(named_for_this_segment(file));
  }
}
