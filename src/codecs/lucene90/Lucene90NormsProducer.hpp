#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSPRODUCER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSPRODUCER_HPP_

#include <optional>
#include <memory>
#include "../../index/SegmentReadState.hpp"
#include "../../index/NumericDocValues.hpp"
#include "../../store/BufferedChecksumMemoryDataInput.hpp"
#include "../../store/DataInput.hpp"
#include "../../util/LinearAllocator.hpp"
#include "IndexedDisi.hpp"
#include "../NormsProducer.hpp"

namespace lucene::cyborg::codecs::v90 {

template<typename T>
struct Lucene90DenseNormsIterator : public index::NumericDocValues {
  explicit Lucene90DenseNormsIterator(int32_t _max_doc)
      : index::NumericDocValues(),
        max_doc(_max_doc),
        doc(-1) {
  }

  bool advance_exact(int32_t target) final {
    doc = target;
    return true;
  }

  int64_t get_cost() final {
    return max_doc;
  }

  int32_t advance(int32_t target) final {
    if (target < max_doc) {
      doc = target;
    }
    return doc = NO_MORE_DOCS;
  }

  int32_t get_doc_id() final {
    return doc;
  }

  int32_t next_doc() final {
    return advance(doc + 1);
  }

  int64_t get_long_value() final {
    auto self = (T *) this;
    return self->do_get_long_value();
  }

  int32_t max_doc;
  int32_t doc;
};

struct Lucene90DenseAllEqualsNormsIterator final :
    public Lucene90DenseNormsIterator<Lucene90DenseAllEqualsNormsIterator> {
  Lucene90DenseAllEqualsNormsIterator(int32_t _max_doc, int64_t _value)
      : Lucene90DenseNormsIterator<Lucene90DenseAllEqualsNormsIterator>(_max_doc),
        value(_value) {
  }

  int64_t do_get_long_value() const noexcept {
    return value;
  }

  int64_t value;
};  // Lucene90DenseAllEqualsNormsIterator

template<int32_t ByteSize>
struct Lucene90DenseByteNormsIterator final :
    public Lucene90DenseNormsIterator<Lucene90DenseByteNormsIterator<ByteSize>> {
  using Super = Lucene90DenseNormsIterator<Lucene90DenseByteNormsIterator<ByteSize>>;

  Lucene90DenseByteNormsIterator(int32_t _max_doc,
                                 const store::DataInput<store::ReadOnlyDataInput> &_slice)
      : Super(_max_doc),
        slice(_slice) {
  }

  int64_t do_get_long_value() {
    static_assert(ByteSize == 1 || ByteSize == 2 || ByteSize == 4 || ByteSize == 8);

    if constexpr (ByteSize == 1) {
      slice.seek(Super::doc);
      return slice.read_byte();
    } else if constexpr (ByteSize == 2) {
      slice.seek(((uint64_t) Super::doc) << 1U);
      return slice.read_short();
    } else if constexpr (ByteSize == 4) {
      slice.seek(((uint64_t) Super::doc) << 2U);
      return slice.read_int();
    } else if constexpr (ByteSize == 8) {
      slice.seek(((uint64_t) Super::doc) << 3U);
      return slice.read_long();
    }  // End if

    // can't reach here
    return -1;
  }

  store::DataInput<store::ReadOnlyDataInput> slice;
};  // Lucene90DenseByte1NormsIterator

template<typename T>
struct Lucene90SparseNormsIterator : public index::NumericDocValues {
  explicit Lucene90SparseNormsIterator(const IndexedDISI &_disi)
      : index::NumericDocValues(),
        disi(_disi) {
  }

  bool advance_exact(int32_t target) final {
    return disi.advance_exact(target);
  }

  int64_t get_long_value() final {
    auto self = (T *) this;
    return self->do_get_long_value();
  }

  int64_t get_cost() final {
    return disi.get_cost();
  }

  int32_t advance(int32_t target) final {
    return disi.advance(target);
  }

  int32_t get_doc_id() final {
    return disi.doc;
  }

  int32_t next_doc() final {
    return disi.next_doc();
  }

  IndexedDISI disi;
};  // Lucene90SparseNormsIterator


struct Lucene90SparseAllEqualNormsIterator final
    : public Lucene90SparseNormsIterator<Lucene90SparseAllEqualNormsIterator> {
  Lucene90SparseAllEqualNormsIterator(const IndexedDISI &_disi, int64_t _value)
      : Lucene90SparseNormsIterator<Lucene90SparseAllEqualNormsIterator>(_disi),
        value(_value) {
  }

  [[nodiscard]] int64_t do_get_long_value() const noexcept {
    return value;
  }

  int64_t value;
};  // Lucene90SparseAllEqualNormsIterator



template<int32_t ByteSize>
struct Lucene90SparseByteNormsIterator final : public Lucene90SparseNormsIterator<Lucene90SparseByteNormsIterator<
    ByteSize>> {
  using Super = Lucene90SparseNormsIterator<Lucene90SparseByteNormsIterator<ByteSize>>;

  Lucene90SparseByteNormsIterator(const IndexedDISI &_disi,
                                  const store::DataInput<store::ReadOnlyDataInput> &_slice)
      : Super(_disi),
        slice(_slice) {
  }

  int64_t do_get_long_value() {
    static_assert(ByteSize == 1 || ByteSize == 2 || ByteSize == 4 || ByteSize == 8);

    if constexpr (ByteSize == 1) {
      slice.seek(Super::disi.index);
      return slice.read_byte();
    } else if constexpr (ByteSize == 2) {
      slice.seek(((uint64_t) Super::disi.index) << 1U);
      return slice.read_short();
    } else if constexpr (ByteSize == 4) {
      slice.seek(((uint64_t) Super::disi.index) << 2U);
      return slice.read_int();
    } else if constexpr (ByteSize == 8) {
      slice.seek(((uint64_t) Super::disi.index) << 3U);
      return slice.read_long();
    }  // End if

    // can't reach here
    return -1;
  }

  store::DataInput<store::ReadOnlyDataInput> slice;
};  // Lucene90SparseAllEqualNormsIterator



class Lucene90NormsProducer final : public codecs::NormsProducer {
 public:
  explicit Lucene90NormsProducer(util::ReadOnlyPtr<index::SegmentReadState> state);

  ~Lucene90NormsProducer() final;

  util::ManagedPtr<index::NumericDocValues> get_norms(util::ManagedPtr<util::LinearAllocator> allocator,
                                                      util::UseOnlyPtr<index::FieldInfo> field) final;

  void close() final;

  void check_integrity();

 private:
  void read_fields(store::DataInput<store::BufferedChecksumMemoryDataInput> &meta,
                   util::UseOnlyPtr<index::FieldInfos> field_infos);

  std::vector<std::optional<Lucene90NormsEntry>> norms;
  int32_t max_doc;
  store::CloseableMemoryDataInput data;
  std::unordered_map<int32_t, store::CloseableMemoryDataInput> disi_inputs;
  std::unordered_map<int32_t, store::CloseableMemoryDataInput> disi_jump_tables;
  std::unordered_map<int32_t, store::CloseableMemoryDataInput> data_inputs;
};  // Lucene90NormsProducer

using Lucene90NormsProducerPtr = std::shared_ptr<Lucene90NormsProducer>;

}  // namespace lucene::cyborg::codec::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSPRODUCER_HPP_