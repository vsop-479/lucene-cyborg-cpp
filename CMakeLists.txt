#######################
## Basic
#######################
cmake_minimum_required(VERSION 3.22)
project(lucene_cyborg_cpp)

set(CMAKE_CXX_STANDARD 20)

#######################
## MACRO START
#######################
macro(add_glob cur_list)
    file(GLOB __tmp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${ARGN})
    list(APPEND ${cur_list} ${__tmp})
endmacro()

macro(add_headers_and_sources prefix common_path)
    add_glob(${prefix}_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${common_path}/*.hpp ${common_path}/*.h)
    add_glob(${prefix}_sources ${common_path}/*.cpp ${common_path}/*.hpp)
    add_glob(${prefix}_sources ${common_path}/*.c ${common_path}/*.h)
endmacro()

#######################
## Compiling options
#######################

if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "Build target=[development].")
    set(CMAKE_CXX_FLAGS "-g -O0 -fPIC")
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    if (NOT WIN32)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif ()
    message(STATUS "Build target=[production].")
    set(CMAKE_CXX_FLAGS "-g -O3 -fPIC -march=native ${PGO_PROFILE} -fvisibility=hidden")
    set(CMAKE_C_FLAGS "-g -O3 -fPIC -march=native ${PGO_PROFILE} -fvisibility=hidden")

    message(STATUS "CMAKE_CXX_FLAGS=[${CMAKE_CXX_FLAGS}]")
    message(STATUS "CMAKE_C_FLAGS=[${CMAKE_C_FLAGS}]")

    add_definitions(-DNDEBUG)
endif ()

#######################
## Build LuceneCyborg
#######################
add_headers_and_sources(LuceneCyborg ./src/codecs)
add_headers_and_sources(LuceneCyborg ./src/codecs/lucene90)
add_headers_and_sources(LuceneCyborg ./src/codecs/lucene90/block_tree)
add_headers_and_sources(LuceneCyborg ./src/codecs/lucene94)
add_headers_and_sources(LuceneCyborg ./src/codecs/lucene95)
add_headers_and_sources(LuceneCyborg ./src/index)
add_headers_and_sources(LuceneCyborg ./src/java)
add_headers_and_sources(LuceneCyborg ./src/pco/factory)
add_headers_and_sources(LuceneCyborg ./src/pco/framework)
add_headers_and_sources(LuceneCyborg ./src/pco/typeindex)
add_headers_and_sources(LuceneCyborg ./src/pco/util)
add_headers_and_sources(LuceneCyborg ./src/search)
add_headers_and_sources(LuceneCyborg ./src/search/query)
add_headers_and_sources(LuceneCyborg ./src/search/query/iterator)
add_headers_and_sources(LuceneCyborg ./src/search/similarity)
add_headers_and_sources(LuceneCyborg ./src/store)
add_headers_and_sources(LuceneCyborg ./src/util)

add_library(LuceneCyborg STATIC ${LuceneCyborg_headers} ${LuceneCyborg_sources})


###########################
## (Optional) Enable JNI
###########################
if (LUCENE_CYBORG_BUILD_JNI)
    add_definitions(-DLUCENE_CYBORG_BUILD_JNI)
    if (DEFINED LUCENE_CYBORG_JNI_INCLUDE_DIRECTORIES)
        message(STATUS "LUCENE_CYBORG_JNI_INCLUDE_DIRECTORIES=[${LUCENE_CYBORG_JNI_INCLUDE_DIRECTORIES}]")
        include_directories(${LUCENE_CYBORG_JNI_INCLUDE_DIRECTORIES})
    endif ()

    message(STATUS "LUCENE_CYBORG_BUILD_JNI is on, adding java/jni/*")
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/aggregator)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/bridge)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/collector)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/common)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/query)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/search)
    add_headers_and_sources(LuceneCyborgJNI ./src/java/jni/similarity)

    add_library(LuceneCyborgJNI SHARED ${LuceneCyborg_headers} ${LuceneCyborgJNI_headers} ${LuceneCyborgJNI_sources})
    target_link_libraries(LuceneCyborgJNI LuceneCyborg)
endif ()


# tantivy_bench_pisa_comparison
add_executable(tantivy_bench_pisa_comparison tantivy_bench_pisa_comparison.cpp)
target_link_libraries(tantivy_bench_pisa_comparison LuceneCyborg)

# tantivy_search_result_validation
add_executable(tantivy_search_result_validation tantivy_search_result_validation.cpp)
target_link_libraries(tantivy_search_result_validation LuceneCyborg)

# tantivy_search_benchmark
add_executable(tantivy_search_benchmark tantivy_search_benchmark.cpp)
target_link_libraries(tantivy_search_benchmark LuceneCyborg)
