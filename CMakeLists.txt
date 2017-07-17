cmake_minimum_required(VERSION 3.5)
project(ifugao)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

find_package(GMP REQUIRED)

set(COMMON_LIBRARIEY ${GMP_LIBRARIES})

include_directories(include)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fomit-frame-pointer -funroll-loops")

# Guard against in-source builds and bad build-type strings
include(ConfigSafeGuards)

## MAIN ###############################################################################

set(CORE_HEADERS
        include/newick-tools/newick-tools.h
        include/newick-tools/parse_ntree.h
        include/types/functional.h
        include/types/leaf_set.h
        include/types/tree.h
        include/types/UnionFind.h
        include/ifugao.h
        include/leaf_label_mapper.h
        include/input_parser.h
        include/terraces.h
        include/util.h)

set(NEWICK_TOOLS_SRCS
        src/newick-tools/lex_ntree.cpp
        src/newick-tools/newick-tools.cpp
        src/newick-tools/ntree.cpp
        src/newick-tools/parse_ntree.cpp
        src/newick-tools/rtree.cpp)

set(CORE_SRCS
        src/types/leaf_set.cpp
        src/types/tree.cpp
        src/types/UnionFind.cpp
        src/ifugao.cpp
        src/input_parser.cpp
        src/leaf_label_mapper.cpp
        src/terraces.cpp
        src/util.cpp)

#CMAKE_CXX_FLAGS is globally used in all projects.
#TARGET_CXX_FLAGS is for own build targets only.
set(TARGET_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(TARGET_CXX_FLAGS "${TARGET_CXX_FLAGS} -Werror -Wall")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(TARGET_CXX_FLAGS "${TARGET_CXX_FLAGS} /WX /W4")
endif()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(TARGET_CXX_FLAGS "${TARGET_CXX_FLAGS} -Werror -Weverything -Wno-shadow \
        -Wno-c++98-compat -Wno-padded -Wno-float-equal -Wno-vla -Wno-weak-vtables")
    set(TEST_CXX_FLAGS "${TARGET_CXX_FLAGS} \
        -Wno-global-constructors -Wno-used-but-marked-unused \
        -Wno-covered-switch-default -Wno-gnu-zero-variadic-macro-arguments \
        -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-disabled-macro-expansion")
endif()

add_library(terraces ${CORE_HEADERS} ${NEWICK_TOOLS_SRCS} ${CORE_SRCS})
# ${NEWICK_TOOLS_SRCS} produces to much warnings, so only check ${CORE_SRCS}
set_source_files_properties(${CORE_SRCS} ${CORE_HEADERS}
        PROPERTIES COMPILE_FLAGS "${TARGET_CXX_FLAGS}")
add_executable(terraces_main src/main.cpp)
set_target_properties(terraces_main PROPERTIES COMPILE_FLAGS "${TARGET_CXX_FLAGS}")
target_link_libraries(terraces_main terraces ${COMMON_LIBRARIEY})

## TESTS & BENCHMARK ##################################################################

include(CTest)
enable_testing()

include(${CMAKE_MODULE_PATH}/DownloadProject/DownloadProject.cmake)

#set(gtest_disable_pthreads on) #needed in MinGW
download_project(
        PROJ                googletest
        GIT_REPOSITORY      https://github.com/google/googletest.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)
include_directories(SYSTEM "${googletest_SOURCE_DIR}/googletest/include")
include_directories(SYSTEM "${googletest_SOURCE_DIR}/googlemock/include")
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

# Disable tests in googlebenchmark
set(BENCHMARK_ENABLE_TESTING OFF)
download_project(
        PROJ                googlebenchmark
        GIT_REPOSITORY      https://github.com/google/benchmark.git
        GIT_TAG             master
        UPDATE_DISCONNECTED 1
)
add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR} EXCLUDE_FROM_ALL)
include_directories(SYSTEM "${googlebenchmark_SOURCE_DIR}/include")

include_directories(test)

set(TEST_HEADERS
        test/parameters.h
        test/util_test.h
        test/algorithm_test.h
        test/input_parser_test.h
        test/terraces_test.h)

add_executable(terraces_tests test/gtest.cpp ${CORE_HEADERS} ${TEST_HEADERS})
set_target_properties(terraces_tests PROPERTIES COMPILE_FLAGS "${TEST_CXX_FLAGS}")
target_link_libraries(terraces_tests terraces ${COMMON_LIBRARIEY} gtest gmock)
add_test(terraces_tests terraces_tests)

add_executable(terraces_benchmarks test/gbenchmark.cpp ${CORE_HEADERS} ${TEST_HEADERS})
set_target_properties(terraces_benchmarks PROPERTIES COMPILE_FLAGS "${TEST_CXX_FLAGS}")
target_link_libraries(terraces_benchmarks terraces ${COMMON_LIBRARIEY} benchmark)
add_test(terraces_benchmarks terraces_benchmarks)
