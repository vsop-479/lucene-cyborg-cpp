#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_MACROS_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_MACROS_HPP_

//
// Always inline
//
#ifdef _MSC_VER
#define LC_ALWAYSINLINE __forceinline
#elif defined(__clang__) || defined(__GNUC__)
#define LC_ALWAYSINLINE inline __attribute__((__always_inline__))
#else
#define LC_ALWAYSINLINE inline
#endif

//
// Built-in prefetch
//
#define LC_PREFETCH(PTR, RW, LOC)    __builtin_prefetch(PTR,RW,LOC)

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_MACROS_HPP_
