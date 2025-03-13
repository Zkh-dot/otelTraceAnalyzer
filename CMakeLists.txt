cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(SOURCE_FILES src/hashstructs/hashmap.c src/hashstructs/hashset.c src/hashstructs/hashset_itr.c src/counters.c src/span.c)

set(CMAKE_BUILD_TYPE Debug) 

add_executable(analyzerV1 ${SOURCE_FILES} src/analyzer.c)


# testing

find_package(GTest REQUIRED)

set(TEST_SOURCE_FILES cTests/allTests.cpp src/hashstructs/hashmap.c src/hashstructs/hashset.c src/counters.c src/span.c  src/hashstructs/hashset_itr.c src/trace.c)

add_executable(tests ${TEST_SOURCE_FILES})
target_link_libraries(tests GTest::GTest GTest::Main)

enable_testing()

add_test(NAME counters_test COMMAND tests)