#include <functional>
#include "Query.hpp"

using lucene::cyborg::search::Query;
using lucene::cyborg::util::ReadOnlyPtr;

//
// Query
//

Query::Query(ReadOnlyPtr<QueryIdentifier> _query_identifier)
    : query_identifier(_query_identifier),
      class_name_hash(std::hash<std::string_view>{}(_query_identifier->class_name)) {
}

bool Query::equals(ReadOnlyPtr<Query> other) const noexcept {
  return other != nullptr && query_identifier == other->query_identifier;
}
