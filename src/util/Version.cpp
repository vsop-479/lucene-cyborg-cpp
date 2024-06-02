#include "Version.hpp"

using lucene::cyborg::util::Version;
using lucene::cyborg::util::ReadOnlyPtr;

//
// Version
//

const Version Version::UNKNOWN = Version(0, 0, 0);
const Version Version::LUCENE_9_8_0 = Version(9, 8, 0);
ReadOnlyPtr<Version> Version::LATEST = &Version::LUCENE_9_8_0;
const int32_t Version::MIN_SUPPORTED_MAJOR = Version::LATEST->major - 1;
