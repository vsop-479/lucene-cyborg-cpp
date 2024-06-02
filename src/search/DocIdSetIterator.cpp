#include "DocIdSetIterator.hpp"

using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::search::DocIdSetIterator;

//
// DocIdSetIterator
//

const TypeInfoPtr DocIdSetIterator::TYPE_INFO =
    TypeInfo::get_type_info<DocIdSetIterator>("DocIdSetIterator");
