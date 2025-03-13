cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(SOURCE_FILES src/hashstructs/hashmap.c src/hashstructs/hashset.c src/counters.c)

add_executable(analyzerV1 ${SOURCE_FILES} src/analyzer.c)


# testing

find_package(GTest REQUIRED)

set(TEST_SOURCE_FILES cTests/allTests.cpp)

add_executable(tests src/hashstructs/hashmap.c src/hashstructs/hashset.c src/counters.c ${TEST_SOURCE_FILES})
target_link_libraries(tests GTest::GTest GTest::Main)

enable_testing()

add_test(NAME counters_test COMMAND tests)