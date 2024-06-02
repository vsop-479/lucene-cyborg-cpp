#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_BRIDGE_DIRECTORYANDSTATICINDEXREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_BRIDGE_DIRECTORYANDSTATICINDEXREADER_HPP_

#ifdef LUCENE_CYBORG_BUILD_JNI

#include <iostream>
#include "lucene_cyborg_jni_StaticMMapDirectoryReader.h"
#include "../common/JString.hpp"
#include "../../../store/MMapDirectory.hpp"
#include "../../../index/MMapDirectoryReader.hpp"

namespace lucene::cyborg::jni {

struct DirectoryAndStaticIndexReader {
  DirectoryAndStaticIndexReader(store::MMapDirectoryPtr &&_dir, index::MMapDirectoryReaderPtr &&_index_reader)
      : dir(std::move(_dir)),
        index_reader(std::move(_index_reader)) {
  }

  store::MMapDirectoryPtr dir;
  index::MMapDirectoryReaderPtr index_reader;
};  // DirectoryAndStaticIndexReader



}  // namespace lucene::cyborg::jni

#endif

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_BRIDGE_DIRECTORYANDSTATICINDEXREADER_HPP_
