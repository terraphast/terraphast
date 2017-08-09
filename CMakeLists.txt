project(terraphast)

cmake_minimum_required(VERSION 3.5)

# create sane environment
#####################################################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# set platform-specific options
if(WIN32)
	include_directories("platform/windows")
	set(BASIC_WARNINGS "-W4")
	set(OPTIMIZATION_FLAGS "-Oi")
	set(GMP_LIBS "")
	set(THREAD_LIBS "")
else()
	include_directories("platform/linux")
	set(BASIC_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion")
	set(OPTIMIZATION_FLAGS "-march=native")
	set(GMP_LIBS gmp gmpxx)
	set(THREAD_LIBS pthread)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASIC_WARNINGS} ${OPTIMIZATION_FLAGS}")

include_directories("include" "c_include")

# build libraries
file(GLOB lib_headers "include/terraces/*.hpp")
file(GLOB lib_sources "lib/*.cpp")
file(GLOB lib_int_headers "lib/*.hpp")
add_library(terraces ${lib_sources} ${lib_headers} ${lib_int_headers})
target_link_libraries(terraces ${THREAD_LIBS})

if(NOT WIN32)
	file(GLOB c_lib_headers "c_include/terraces/*.h")
	file(GLOB c_lib_sources "c_lib/*.cpp")
	file(GLOB c_lib_int_headers "c_lib/*.hpp")
	add_library(c_terraces ${c_lib_sources} ${c_lib_headers} ${c_lib_int_headers})
	target_link_libraries(c_terraces terraces)
	target_link_libraries(terraces ${THREAD_LIBS})
endif()

# build tests
Option(BUILD_TESTS "BUILD_TESTS" OFF)
Option(BUILD_TESTS_FULL "BUILD_TESTS_FULL" OFF)
if(BUILD_TESTS OR BUILD_TESTS_FULL)
	include_directories("Catch/include")
	file(GLOB test_headers "test/*.hpp")
	file(GLOB test_sources "test/*.cpp")
	file(GLOB test_full_sources "test/full/*.cpp")
	if (BUILD_TESTS_FULL)
		add_executable(unittests ${test_sources} ${test_full_sources} ${test_headers})
	else()
		add_executable(unittests ${test_sources} ${test_headers})
	endif()
	target_link_libraries(unittests terraces ${GMP_LIBS} ${THREAD_LIBS})
	add_test(NAME unittests COMMAND unittests)
	enable_testing()
endif()

file(GLOB app_sources "app/*.cpp")
file(GLOB tool_sources "tools/*.cpp")

# build example app
add_executable(app "app/main.cpp")
add_executable(progress-app "app/progress-app.cpp")
target_link_libraries(app terraces ${GMP_LIBS} ${THREAD_LIBS})
target_link_libraries(progress-app terraces ${GMP_LIBS} ${THREAD_LIBS})

# build helper tools
add_executable(isomorphic "tools/isomorphic.cpp")
add_executable(reroot "tools/reroot.cpp")
add_executable(subtree "tools/subtree.cpp")
add_executable(tree_gen "tools/tree_gen.cpp")
add_executable(site_gen "tools/site_gen.cpp")
target_link_libraries(isomorphic terraces)
target_link_libraries(reroot terraces)
target_link_libraries(subtree terraces)
target_link_libraries(tree_gen terraces)
target_link_libraries(site_gen terraces)

# Most of our files only compile with disabled language extensions for VC++
# Unfortunately, Catch uses some windows-specific features, so we have to
# enable these extensions for the Catch main method (more specific: not disable them)
if(BUILD_TESTS AND WIN32)
	file(GLOB test_main_source "test/main.cpp")
	list(REMOVE_ITEM test_sources ${test_main_source})
	set_source_files_properties(${lib_sources} ${test_sources} ${app_sources} ${tool_sources} PROPERTIES COMPILE_FLAGS "-Za")
endif()

# Use debug version of STL
Option(DEV_ENVIRONMENT "DEV_ENVIRONMENT" OFF)
if(DEV_ENVIRONMENT AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -Werror")
endif()
