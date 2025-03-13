cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(LIB_FILES  
    src/hashstructs/hashmap.c
    src/hashstructs/hashset.c
    src/hashstructs/hashset_itr.c

    src/structures/counters.c
    src/structures/span.c
    src/structures/trace.c
    src/structures/service.c
    src/structures/servicemap.c
)


set(CMAKE_BUILD_TYPE Debug) 

add_executable(analyzerV1 ${LIB_FILES} src/analyzer.c)

set_property(TARGET analyzerV1 PROPERTY C_STANDARD 23)

# testing

find_package(GTest REQUIRED)

add_executable(tests ${LIB_FILES} cTests/allTests.cpp)
target_link_libraries(tests GTest::GTest GTest::Main)

enable_testing()

add_test(NAME counters_test COMMAND tests)

set_property(TARGET tests PROPERTY C_STANDARD 23)
