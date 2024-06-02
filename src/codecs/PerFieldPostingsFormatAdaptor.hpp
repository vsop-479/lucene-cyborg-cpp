#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_PERFIELDPOSTINGSFORMATADAPTOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_PERFIELDPOSTINGSFORMATADAPTOR_HPP_

#include <memory>
#include <memory>
#include "../index/SegmentReadState.hpp"

namespace lucene::cyborg::codecs {

template<typename PostingsFormat>
struct PerFieldPostingsFormatAdaptor {
  typename PostingsFormat::FieldsProducerPtr fields_producer(util::ReadOnlyPtr<index::SegmentReadState> read_state) {
    static const std::string PER_FIELD_FORMAT_KEY = "PerFieldPostingsFormat.format";
    static const std::string PER_FIELD_SUFFIX_KEY = "PerFieldPostingsFormat.suffix";

    std::string first_segment_suffix;

    // Read field name -> format name
    for (auto &fi : read_state->field_infos->values) {
      if (fi->index_options != index::IndexOptions::NONE) {
        const auto &field_name = fi->name;
        auto format_name_it = fi->attributes.find(PER_FIELD_FORMAT_KEY);
        // null format_name means the field is fst_in field_infos, but has no postings!
        if (format_name_it != fi->attributes.end()) {
          const auto &format_name = format_name_it->second;
          auto suffix_it = fi->attributes.find(PER_FIELD_SUFFIX_KEY);
          if (suffix_it == fi->attributes.end()) {
            // TODO
            throw 13;
          }
          const auto &suffix = suffix_it->second;
          std::string segment_suffix = PostingsFormat::NAME + "_" + suffix;

          if (first_segment_suffix.empty()) {
            first_segment_suffix = std::move(segment_suffix);
          } else if (first_segment_suffix != segment_suffix) {
            // TODO
            throw 13;
          }
        }
      }
    }  // End for

    index::SegmentReadState new_read_state{read_state->directory,
                                           read_state->segment_info,
                                           read_state->field_infos,
                                           first_segment_suffix};

    return PostingsFormat{}.fields_producer(&new_read_state);
  }
};  // PerFieldPostingsFormatAdaptor



}  // lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_PERFIELDPOSTINGSFORMATADAPTOR_HPP_
