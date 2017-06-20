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
	#set(OPTIMIZATION_FLAGS "${OPTIMIZATION_FLAGS} -arch:AVX")
else()
	include_directories("platform/linux")
	set(BASIC_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion")
	set(OPTIMIZATION_FLAGS "-march=native")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASIC_WARNINGS} ${OPTIMIZATION_FLAGS}")

include_directories("include")
enable_testing()


Option(DEV_ENVIRONMENT "DEV_ENVIRONMENT" OFF)
if(DEV_ENVIRONMENT AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -Werror")
endif()

# Get Libraries
#####################################################################

# TODO add proper use of GMP

#####################################################################

file(GLOB lib_headers "include/terraces/*.hpp")
file(GLOB lib_sources "lib/*.cpp")
file(GLOB lib_int_headers "lib/*.hpp")
add_library(terraces ${lib_sources} ${lib_headers} ${lib_int_headers})
target_link_libraries(terraces gmp gmpxx pthread)
Option(ASM_OUTPUT "ASM_OUTPUT" OFF)
if(ASM_OUTPUT)
	target_compile_options(terraces PRIVATE "-save-temps")
endif()


file(GLOB test_headers "test/*.hpp")
file(GLOB test_sources "test/*.cpp")
add_executable(unittests ${test_sources} ${test_headers})
target_link_libraries(unittests terraces gmp gmpxx)
add_test(NAME unittests COMMAND unittests)

file(GLOB app_headers "app/*.hpp")
file(GLOB app_sources "app/*.cpp")

add_executable(app "app/main.cpp")
target_link_libraries(app terraces gmp gmpxx)

add_executable(convert "app/convert.cpp")
target_link_libraries(convert terraces)

add_executable(isomorphic "app/isomorphic.cpp")
target_link_libraries(isomorphic terraces)

# Most of our files only compile with disabled language extensions for VC++
# Unfortunately, Catch uses some windows-specific features, so we have to
# enable these extensions for the Catch main method (more specific: not disable them)
if(WIN32)
	file(GLOB test_main_source "test/main.cpp")
	list(REMOVE_ITEM test_sources ${test_main_source})
	set_source_files_properties(${lib_sources} ${test_sources} ${app_sources} PROPERTIES COMPILE_FLAGS "-Za")
endif()
