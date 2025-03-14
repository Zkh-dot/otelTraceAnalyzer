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

    src/analyzer.c
)


set(CMAKE_BUILD_TYPE Debug) 

# python .so build

find_package(Python3 COMPONENTS Development REQUIRED)

Python3_add_library(analyzer SHARED ${LIB_FILES} src/python_wrapper/py_analyzer.c)

# mem test

add_executable(memtest ${LIB_FILES} cTests/mem_test.c)
set_property(TARGET memtest PROPERTY C_STANDARD 23)

# testing

find_package(GTest REQUIRED)

add_executable(struct_tests ${LIB_FILES} cTests/struct_tests.cpp)
target_link_libraries(struct_tests GTest::GTest GTest::Main)

add_executable(analyzer_tests ${LIB_FILES} cTests/analyzer_tests.cpp)
target_link_libraries(analyzer_tests GTest::GTest GTest::Main)

enable_testing()

add_test(NAME struct_tests COMMAND struct_tests)
add_test(NAME analyzer_tests COMMAND analyzer_tests)

set_property(TARGET struct_tests PROPERTY C_STANDARD 23)
set_property(TARGET analyzer_tests PROPERTY C_STANDARD 23)
