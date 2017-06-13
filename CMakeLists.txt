project(terrace-searcher)

cmake_minimum_required(VERSION 3.5)


# create sane environment
#####################################################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BASIC_WARNINGS "-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion")
set(OPTIMIZATION_FLAGS "-march=native")
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
target_link_libraries(terraces gmp gmpxx)
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
add_executable(app ${app_sources} ${app_headers})
target_link_libraries(app terraces gmp gmpxx)
