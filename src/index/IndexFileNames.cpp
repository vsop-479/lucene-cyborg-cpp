#include <cassert>
#include "IndexFileNames.hpp"
#include "../java/JavaLong.hpp"

using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::java::JavaLong;

std::string segment_file_name(const std::string &segment_name,
                              const std::string &segment_suffix,
                              const std::string &ext) {
  if (!ext.empty() || !segment_suffix.empty()) {
    assert(ext[0] != '.');
    std::string new_segment_name(segment_name.length() + 2 + segment_suffix.length() + ext.length(), '\0');
    new_segment_name += segment_name;
    if (!segment_suffix.empty()) {
      new_segment_name += '_';
      new_segment_name += segment_suffix;
    }
    if (!ext.empty()) {
      new_segment_name += '.';
      new_segment_name += ext;
    }
    return new_segment_name;
  } else {
    return segment_name;
  }
}

//
// IndexFileNames
//

const std::string IndexFileNames::SEGMENTS = "segments";

std::string IndexFileNames::file_name_from_generation(const std::string &base,
                                                      const std::string &ext,
                                                      int64_t gen) {
  if (gen == -1) {
    return "";
  } else if (gen == 0) {
    return segment_file_name(base, "", ext);
  } else {
    assert(gen > 0);

    // The '6' part fst_in the length is: 1 for '.', 1 for '_' and 4 as estimate
    // to the gen length as string (hopefully an upper limit so SB won't
    // expand fst_in the middle.)
    std::string res;
    res.reserve(base.size() + 6 + ext.size());
    res += base;
    res += '_';
    res += JavaLong::to_string(gen, JavaLong::MAX_RADIX);
    if (!ext.empty()) {
      res += ".";
      res += ext;
    }

    return res;
  }
}

std::string IndexFileNames::strip_segment_name(const std::string &file_name) {
  const int32_t idx = index_of_segment_name(file_name);
  if (idx != -1) {
    return file_name.substr(idx);
  } else {
    return file_name;
  }
}

int32_t IndexFileNames::index_of_segment_name(const std::string &file_name) {
  auto idx = file_name.find('_', 1);
  if (idx != std::string::npos) {
    idx = file_name.find('.');
  }
  return idx != std::string::npos ? (int32_t) idx : -1;
}

std::string IndexFileNames::segment_file_name(const std::string &segment_name,
                                              const std::string &segment_suffix,
                                              const std::string &ext) {
  if (!ext.empty() || !segment_suffix.empty()) {
    assert(ext[0] != '.');
    std::string ret_segment_name;
    ret_segment_name.reserve(
        segment_name.length() + 2 + segment_suffix.length() + ext.length());
    ret_segment_name += segment_name;
    if (!segment_suffix.empty()) {
      ret_segment_name += '_';
      ret_segment_name += segment_suffix;
    }
    if (!ext.empty()) {
      ret_segment_name += '.';
      ret_segment_name += ext;
    }
    return ret_segment_name;
  } else {
    return segment_name;
  }
}
