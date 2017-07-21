project(terrace-searcher)

cmake_minimum_required(VERSION 3.5)

# create sane environment
#####################################################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
	include_directories("platform/windows")
	set(BASIC_WARNINGS "-W4 -WX")
	set(OPTIMIZATION_FLAGS "-Oi")
	set(GMP_LIBS "")
	set(THREAD_LIBS "")
	#set(OPTIMIZATION_FLAGS "${OPTIMIZATION_FLAGS} -arch:AVX")
else()
	include_directories("platform/linux")
	set(BASIC_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion")
	set(OPTIMIZATION_FLAGS "-march=native")
	set(GMP_LIBS gmp gmpxx)
	set(THREAD_LIBS pthread)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASIC_WARNINGS} ${OPTIMIZATION_FLAGS}")

include_directories("include" "Catch/include")
enable_testing()

# Get Libraries
#####################################################################

# TODO add proper use of GMP

#####################################################################
file(GLOB lib_headers "include/terraces/*.hpp")
file(GLOB lib_sources "lib/*.cpp")
file(GLOB lib_int_headers "lib/*.hpp")
add_library(terraces ${lib_sources} ${lib_headers} ${lib_int_headers})
target_link_libraries(terraces ${THREAD_LIBS})

file(GLOB test_headers "test/*.hpp")
file(GLOB test_sources "test/*.cpp")
add_executable(unittests ${test_sources} ${test_headers})
target_link_libraries(unittests terraces ${GMP_LIBS} ${THREAD_LIBS})
add_test(NAME unittests COMMAND unittests)

file(GLOB app_headers "app/*.hpp")
file(GLOB app_sources "app/*.cpp")

add_executable(app "app/main.cpp")
target_link_libraries(app terraces ${GMP_LIBS} ${THREAD_LIBS})

add_executable(progress-app "app/progress-app.cpp")
target_link_libraries(progress-app terraces ${GMP_LIBS} ${THREAD_LIBS})

add_executable(convert "app/convert.cpp")
target_link_libraries(convert terraces ${THREAD_LIBS})

add_executable(isomorphic "app/isomorphic.cpp")
target_link_libraries(isomorphic terraces ${THREAD_LIBS})

add_executable(reroot "app/reroot.cpp")
target_link_libraries(reroot terraces ${THREAD_LIBS})

add_executable(subtree "app/subtree.cpp")
target_link_libraries(subtree terraces ${THREAD_LIBS})

add_executable(tree_gen "app/tree_gen.cpp")
target_link_libraries(tree_gen terraces ${THREAD_LIBS})

add_executable(site_gen "app/site_gen.cpp")
target_link_libraries(site_gen terraces ${THREAD_LIBS})


# Most of our files only compile with disabled language extensions for VC++
# Unfortunately, Catch uses some windows-specific features, so we have to
# enable these extensions for the Catch main method (more specific: not disable them)
if(WIN32)
	file(GLOB test_main_source "test/main.cpp")
	list(REMOVE_ITEM test_sources ${test_main_source})
	set_source_files_properties(${lib_sources} ${test_sources} ${app_sources} PROPERTIES COMPILE_FLAGS "-Za")
endif()

# Test coverage generation
Option(GCOV_OUTPUT "GCOV_OUTPUT" OFF)
if(GCOV_OUTPUT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-dir=${CMAKE_BINARY_DIR}")
	add_custom_command(
		 TARGET unittests
		 COMMENT "Generate test coverage statistics"
		 POST_BUILD
		 WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		 COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> -R "unittests" --output-on-failures
		 COMMAND lcov --no-external --base-directory ${CMAKE_SOURCE_DIR} --capture --directory . --output-file coverage.info
		 COMMAND genhtml coverage.info --output-directory out
	)
endif()

# Assembly generation
Option(ASM_OUTPUT "ASM_OUTPUT" OFF)
if(ASM_OUTPUT)
	target_compile_options(terraces PRIVATE "-save-temps")
endif()

# Use debug version of STL
Option(DEV_ENVIRONMENT "DEV_ENVIRONMENT" OFF)
if(DEV_ENVIRONMENT AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -Werror")
endif()
