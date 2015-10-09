cmake_minimum_required(VERSION 2.6)
project(embedlib)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wformat -Og -g")

# Override for your CppUTest directory (must be pre-built)
set(CPPUTEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../cpputest/include)
set(CPPUTEST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../cpputest/src/CppUTest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CPPUTEST_INCLUDE_DIR})
link_directories(${CPPUTEST_LIB_DIR})

set(SOURCE 
	src/string_wrapper.cpp
	src/heap_string.cpp
	)

add_library(embedlib SHARED ${SOURCE})

# Unit tests
set(TEST_SOURCE 
	test/main.cpp
	test/string_wrapper_test.cpp
	test/heap_string_test.cpp
	test/stack_string_test.cpp)
	
add_executable(embedlib_u ${TEST_SOURCE})
target_link_libraries(embedlib_u embedlib CppUTest)
