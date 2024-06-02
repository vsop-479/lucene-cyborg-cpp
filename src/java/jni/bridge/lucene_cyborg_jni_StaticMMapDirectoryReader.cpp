#ifdef LUCENE_CYBORG_BUILD_JNI

#include <iostream>
#include "lucene_cyborg_jni_StaticMMapDirectoryReader.h"
#include "../common/JString.hpp"
#include "DirectoryAndStaticIndexReader.hpp"

using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::index::MMapDirectoryReaderPtr;
using lucene::cyborg::jni::DirectoryAndStaticIndexReader;
using lucene::cyborg::jni::JString;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;

JNIEXPORT jlong JNICALL Java_lucene_cyborg_jni_StaticMMapDirectoryReader_openMmapDirectoryReader
    (JNIEnv *env, jclass static_mmap_directory_reader_proxy, jstring path) {
  JString path_jstr{env, path};
  const std::string path_std_str{path_jstr.get_string()};
  MMapDirectoryPtr directory = std::make_shared<MMapDirectory>(path_std_str);
  auto index_reader = MMapDirectoryReader::open(directory);
  return (jlong) (new DirectoryAndStaticIndexReader(std::move(directory), std::move(index_reader)));
}

JNIEXPORT void JNICALL Java_lucene_cyborg_jni_StaticMMapDirectoryReader_closeMmapDirectoryReader
    (JNIEnv *, jclass static_mmap_directory_reader_proxy, jlong pointer) {
  auto directory_index_reader_holder = (DirectoryAndStaticIndexReader *) pointer;
  delete directory_index_reader_holder;
}

#endif
