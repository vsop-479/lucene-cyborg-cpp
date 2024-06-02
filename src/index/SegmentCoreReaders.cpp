#include "SegmentCoreReaders.hpp"
#include "../codecs/EffectiveCodecs.hpp"

using lucene::cyborg::index::SegmentCoreReaders;
using lucene::cyborg::index::SegmentCommitInfoPtr;
using lucene::cyborg::index::SegmentReadState;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::codecs::CurrentCodec;
using lucene::cyborg::codecs::PreviousCodec;

//
// SegmentCoreReaders
//

SegmentCoreReaders::SegmentCoreReaders(MMapDirectoryPtr _dir, SegmentCommitInfoPtr _si) {
  if (_si->info->is_compound_file) {
    // TODO : Impl
    throw 13;
  } else {

  }  // End if

  segment = _si->info->name;
  const bool use_current_codec = _si->info->codec_name == CurrentCodec::CODE_NAME;

  // Field info
  if (use_current_codec) {
    core_field_infos = CurrentCodec::FieldInfosFormat().read(_dir, _si->info, "");
  } else {
    core_field_infos = PreviousCodec::FieldInfosFormat().read(_dir, _si->info, "");
  }

  SegmentReadState read_state{_dir, _si->info, core_field_infos, ""};

  // Norms
  if (use_current_codec) {
    norms_producer = CurrentCodec::NormsFormat().create_norms_producer(&read_state);
  } else {
    // TODO
    assert(false);
  }

  // Postings
  if (core_field_infos->has_postings) {
    if (use_current_codec) {
      fields = CurrentCodec::PostingsFormat().fields_producer(&read_state);
    } else {
      fields = PreviousCodec::PostingsFormat().fields_producer(&read_state);
    }
  }
}
