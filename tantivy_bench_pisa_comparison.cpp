#include <string>
#include <chrono>
#include <iostream>
#include <fstream>

#include "./src/store/MMapDirectory.hpp"
#include "./src/index/MMapDirectoryReader.hpp"
#include "./src/search/IndexSearcher.hpp"
#include "./src/search/MatchCountCollectorManager.hpp"
#include "./src/search/SimpleTopScoreDocCollectorManager.hpp"
#include "./src/codecs/lucene90/block_tree/SegmentTermsEnum.hpp"
#include "./src/codecs/lucene90/Lucene90PostingsEnum.hpp"
#include "./src/search/query/TermQuery.hpp"
#include "./src/search/query/BooleanQuery.hpp"
#include "src/util/LinearAllocatorAdaptorForSTL.hpp"

using lucene::cyborg::codecs::v90::Lucene90PostingsEnum;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnum;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::index::MMapDirectoryReaderPtr;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::Terms;
using lucene::cyborg::search::Collector;
using lucene::cyborg::search::CollectorManager;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::MatchCountCollectorManager;
using lucene::cyborg::search::ScoreDoc;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::SimpleTopScoreDocCollectorManager;
using lucene::cyborg::search::query::BooleanClause;
using lucene::cyborg::search::query::BooleanQuery;
using lucene::cyborg::search::query::BooleanQueryBuilder;
using lucene::cyborg::search::query::TermQuery;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::microseconds;

struct Cursor {
  ManagedPtr<SegmentTermsEnum> segment_terms_enum;
  ManagedPtr<Lucene90PostingsEnum<IndexOptions::DOCS_AND_FREQS>> postings_enum;
};  // Cursor

int32_t disjunction(LVector<Cursor> &cursors) {
  int32_t min_doc_id = DocIdSetIterator::NO_MORE_DOCS;
  for (auto &cursor : cursors) {
    if (auto doc_id = cursor.postings_enum->next_doc(); doc_id < min_doc_id) {
      min_doc_id = doc_id;
    }
  }

  int32_t cnt = 0;
  int32_t cur_doc = min_doc_id;
  while (cur_doc != DocIdSetIterator::NO_MORE_DOCS) {
    ++cnt;
    int32_t next_doc = DocIdSetIterator::NO_MORE_DOCS;
    for (auto &cursor : cursors) {
      if (cursor.postings_enum->get_doc_id() == cur_doc) {
        cursor.postings_enum->next_doc();
      }

      next_doc = std::min(next_doc, cursor.postings_enum->get_doc_id());
    }
    cur_doc = next_doc;
  }  // End while

  return cnt;
}

int32_t conjunction(LVector<Cursor> &cursors) {
  std::sort(cursors.begin(), cursors.end(), [](auto &lhs, auto &rhs) {
    return lhs.segment_terms_enum->doc_freq() < rhs.segment_terms_enum->doc_freq();
  });

  auto lead1 = cursors[0].postings_enum;
  int doc = lead1->advance(0);
  auto lead2 = cursors[1].postings_enum;
  int count = 0;

  while (true) {
    while (true) {
      ADVANCE_HEAD:

      const int32_t next2 = lead2->advance(doc);
      if (next2 != doc) {
        doc = lead1->advance(next2);
        if (next2 != doc) {
          goto ADVANCE_HEAD;
        }
      }

      for (int32_t i = 2; i < cursors.size(); ++i) {
        auto other = cursors[i].postings_enum;
        if (other->get_doc_id() < doc) {
          const int32_t next = other->advance(doc);

          if (next > doc) {
            doc = lead1->advance(next);
            goto ADVANCE_HEAD;
          }
        }
      }

      break;
    }  // End while

    if (doc != DocIdSetIterator::NO_MORE_DOCS) {
      ++count;
    } else {
      break;
    }
  }  // End while

  return count;
}

int main() {
  std::string index_path = "./reordered_idx";

  // TMP
//  index_path = "C:\\Users\\ipxds\\workspace\\lucene-cyborg-search-benchmark-game\\engines\\lucene-9.8.0-reordered\\idx";
  // TMP

  MMapDirectoryPtr dir = std::make_shared<MMapDirectory>(index_path);
  auto index_reader = MMapDirectoryReader::open(dir);
  auto segment_reader = index_reader->sub_readers[0];
  SearchContext root_search_ctx{};
  auto text_field_terms = segment_reader->segment_core_readers->fields->terms("text", &root_search_ctx);

  constexpr char DELIMITER = '\t';

  std::string line;
  while (std::getline(std::cin, line)) {
//  while (true) {
//    line = "COUNT\t+griffith +observatory";

    std::string_view read_line = line;
    int i = 0;
    for (int until = line.size(); i < until; ++i) {
      if (line[i] == DELIMITER) {
        break;
      }
    }

    std::string_view command = read_line.substr(0, i);
    std::string_view query_str = read_line.substr(i + 1, read_line.size() - i - 1);

    if (command == "COUNT") {
      LinearAllocator allocator{};
      LVector<Cursor> cursors{
          LinearAllocatorAdapterForSTL<Cursor>{&allocator}};
      bool is_conjunction = false;

      int j = 0;
      for (int k = 0; k <= query_str.size(); ++k) {
        if (k == query_str.size() || query_str[k] == ' ') {
          auto segment_terms_enum = text_field_terms->iterator(&allocator);
          if (query_str[j] == '+') {
            is_conjunction = true;
            bool result = segment_terms_enum->seek_exact(query_str.substr(j + 1, k - j - 1));
            auto postings_enum = segment_terms_enum->postings(PostingsEnum::NONE);
            cursors.emplace_back(Cursor{(SegmentTermsEnum *) segment_terms_enum,
                                        (Lucene90PostingsEnum<IndexOptions::DOCS_AND_FREQS> *) postings_enum});
          } else {
            bool result = segment_terms_enum->seek_exact(query_str.substr(j, k - j));
            auto postings_enum = segment_terms_enum->postings(PostingsEnum::NONE);
            cursors.emplace_back(Cursor{(SegmentTermsEnum *) segment_terms_enum,
                                        (Lucene90PostingsEnum<IndexOptions::DOCS_AND_FREQS> *) postings_enum});
          }

          j = k + 1;
        }  // End if
      }  // End for

      if (is_conjunction) {
        const auto cnt = conjunction(cursors);
        std::cout << cnt << '\n';
      } else {
        const auto cnt = disjunction(cursors);
        std::cout << cnt << '\n';
      }
    } else {
      std::cout << "UNSUPPORTED\n";
    }  // End if
  }  // End while
}
