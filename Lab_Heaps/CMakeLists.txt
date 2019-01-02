cmake_minimum_required(VERSION 3.12)
project(Heaps)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(Heaps main.cpp Tests/tests.h Tests/tests.cpp Vector.h Heap.h BinomialHeap.h)
target_link_libraries(Heaps gtest gtest_main)