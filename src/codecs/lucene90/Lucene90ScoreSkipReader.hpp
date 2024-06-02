#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SCORESKIPREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SCORESKIPREADER_HPP_

#include "Lucene90SkipReader.hpp"
#include "../../index/Impacts.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../util/MemoryUtils.hpp"
#include "../../store/DataInput.hpp"

namespace lucene::cyborg::codecs::v90 {

struct Lucene90Impacts final : public index::Impacts {
  Lucene90Impacts(util::ReadOnlyPtr<int32_t> _num_levels,
                  util::ManagedPtr<int32_t> _skip_doc,
                  util::ManagedPtr<int32_t> _impact_data_length,
                  util::ManagedPtr<util::TypedReadOnlyVector<uint8_t>> _impact_data,
                  util::ManagedPtr<util::LVector<index::Impact>> _per_level_impacts)
      : index::Impacts(_num_levels),
        skip_doc(_skip_doc),
        impact_data_length(_impact_data_length),
        impact_data(_impact_data),
        per_level_impacts(_per_level_impacts),
        impact_data_in() {
  }

  int32_t get_doc_id_up_to(int32_t level) final {
    assert(level < Lucene90Constants::MAX_SKIP_LEVELS);
    return skip_doc[level];
  }

  util::TypedVector<index::Impact> get_impacts(int32_t level) final {
    assert(level < *num_levels);
    if (impact_data_length[level] > 0) {
      impact_data_in.input.data = impact_data[level].vec;
      impact_data_in.input.length = impact_data[level].len;
      read_impacts(impact_data_in, per_level_impacts[level]);
      impact_data_length[level] = 0;
    }  // End if

    return util::TypedVector<index::Impact>{
        per_level_impacts[level].data(),
        (int32_t) per_level_impacts[level].size()
    };
  }

  static void read_impacts(store::DataInput<store::ReadOnlyDataInput> impact_data_in,
                           util::LVector<index::Impact> &reuse) {
    const int32_t max_num_impacts = impact_data_in.input.length;
    int32_t freq = 0;
    int64_t norm = 0;

    if (reuse.capacity() < max_num_impacts) {
      reuse.reserve(util::MemoryUtils::oversize(max_num_impacts));
    }

    reuse.clear();

    for (auto until = impact_data_in.input.data + impact_data_in.input.length; impact_data_in.input.data != until;) {
      uint32_t freq_delta = impact_data_in.read_vint();
      if (freq_delta & 0x01U) {
        freq += 1 + (freq_delta >> 1U);
        try {
          norm += 1 + impact_data_in.read_zlong();
        } catch (...) {
          // TODO
          throw 13;
          // throw new RuntimeException(e); // cannot happen on a BADI
        }
      } else {
        freq += 1 + (freq_delta >> 1U);
        ++norm;
      }  // End if

      reuse.emplace_back(index::Impact{freq, norm});
    }  // End for
  }

  util::ManagedPtr<int32_t> skip_doc;
  util::ManagedPtr<int32_t> impact_data_length;
  util::ManagedPtr<util::TypedReadOnlyVector<uint8_t>> impact_data;
  util::ManagedPtr<util::LVector<index::Impact>> per_level_impacts;
  store::DataInput<store::ReadOnlyDataInput> impact_data_in;
};  // Lucene90Impacts



template<index::IndexOptions OptionIndex>
class Lucene90ScoreSkipReader final : public Lucene90SkipReader<OptionIndex> {
  using Super = Lucene90SkipReader<OptionIndex>;

 public:
  Lucene90ScoreSkipReader(util::UseOnlyPtr<util::LinearAllocator> _allocator,
                          const store::DataInput<store::ReadOnlyDataInput> &_skip_in,
                          const int32_t _df,
                          const int64_t _first_skip_pointer,
                          const int64_t _doc_base_pointer,
                          const int64_t _pos_base_pointer = -1,
                          const bool _has_payload = false,
                          const int64_t _pay_base_pointer = -1)
      : Super(_allocator,
              _skip_in,
              _df,
              _first_skip_pointer,
              _doc_base_pointer,
              _pos_base_pointer,
              _has_payload,
              _pay_base_pointer),
        impact_data(),
        impact_data_length(),
        per_level_impacts(),
        impacts(&num_levels, Super::skip_doc, impact_data_length, impact_data, per_level_impacts),
        num_levels(-1) {
    // Init `per_level_impact`
    for (auto &per_level_impact : per_level_impacts) {
      per_level_impact = util::LVector<index::Impact>{
          util::LinearAllocatorAdapterForSTL<index::Impact>(_allocator)};
      per_level_impact.emplace_back(index::Impact{INT32_MAX, 1});
    }
  }

  int32_t skip_to(const int32_t target) {
    const int32_t result = Super::skip_to(target);

    if (Super::number_of_skip_levels > 0) {
      num_levels = Super::number_of_skip_levels;
    } else {
      // End of postings don't have skip data anymore, so we fill with dummy data
      // like SlowImpactsEnum.
      num_levels = 1;
      per_level_impacts[0].resize(1);
      per_level_impacts[0][0] = {std::numeric_limits<int32_t>::max(), 1L};
      impact_data_length[0] = 0;
    }

    return result;
  }

  util::ManagedPtr<Lucene90Impacts> get_impacts() noexcept {
    return &impacts;
  }

 private:
  void read_impacts(const int32_t level, store::DataInput<store::ReadOnlyDataInput> &skip_in) final {
    const int32_t length = skip_in.read_vint();
    impact_data[level].vec = skip_in.input.data;
    impact_data[level].len = length;
    skip_in.skip_bytes(length);
    impact_data_length[level] = length;
  }

  util::TypedReadOnlyVector<uint8_t> impact_data[Lucene90Constants::MAX_SKIP_LEVELS];
  int32_t impact_data_length[Lucene90Constants::MAX_SKIP_LEVELS];
  util::LVector<index::Impact> per_level_impacts[Lucene90Constants::MAX_SKIP_LEVELS];
  Lucene90Impacts impacts;
  int32_t num_levels;
};  // Lucene90ScoreSkipReader



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SCORESKIPREADER_HPP_
