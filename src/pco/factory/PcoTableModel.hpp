#ifndef LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTABLEMODEL_HPP_
#define LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTABLEMODEL_HPP_

#include "../../util/Macros.hpp"
#include <cstddef>
#include <array>

namespace lucene::cyborg::pco {

template<int N>
struct PcoTableKey {
  std::array<int, N> type_indices;
  size_t hash;

  PcoTableKey() = default;

  PcoTableKey(std::array<int, N> &&_type_indices, int offset = 0) noexcept
      : type_indices(std::move(_type_indices)),
        hash() {
    static constexpr size_t PRIME = 31;
    size_t m = 1;
    for (int i = 0; i < N; ++i) {
      LC_PREFETCH(&type_indices[i], 0, 0);
      hash += m * type_indices[i];
      m *= PRIME;
    }
    hash += m + offset;
  }

  bool operator==(const PcoTableKey &other) const noexcept {
    return hash == other.hash && type_indices == other.type_indices;
  }
};  // PcoTableKey



template<typename FactoryFunc, typename Key>
struct PcoTableCell {
  int next_index;
  FactoryFunc factory_func;
  Key key;

  PcoTableCell()
      : next_index(-1),
        factory_func(),
        key() {
  }

  PcoTableCell(int _next_index, FactoryFunc _factory_func, Key &&_key)
      : next_index(_next_index),
        factory_func(_factory_func),
        key(std::move(_key)) {
  }
};  // PcoTableCell



struct PcoTableLookUp {
  template<typename Key, typename TableCell, int TableSize>
  static auto look_up(const Key &key, TableCell table[]) {
    int index = key.hash % TableSize;
    // locate first element
    while (true) {
      LC_PREFETCH(&table[index], 0, 0);
      if (table[index].next_index == -1) {
        index = (index + 1) % TableSize;
      } else {
        break;
      }
    }

    // find table cell
    while (true) {
      LC_PREFETCH(&table[index], 0, 0);
      if (key == table[index].key) {
        return table[index].factory_func;
      }
      index = table[index].next_index;
    }  // End while
  }
};  // PcoTableLookUp



}  // namespace lucene::cyborg::pco

#endif //LUCENE_CYBORG_CPP_SRC_PCO_FACTORY_PCOTABLEMODEL_HPP_
