cmake_minimum_required(VERSION 3.15)
project(otelanalyzer C)

set(CMAKE_C_STANDARD 23)

# Create a Python module (shared library)
set(SOURCE_FILES
    src/analyzer.c

    src/plugin_manager.c
    src/plugins/example_plugin.c

    src/hashstructs/hashset_itr.c
    src/hashstructs/hashmap.c
    src/hashstructs/hashset.c

    src/python_wrapper/py_structs.c
    src/python_wrapper/py_translator.c
    src/python_wrapper/py_analyzer.c
    src/python_wrapper/py_plugin_manager.c

    src/python_wrapper/init_module.c

    src/structures/servicemap.c
    src/structures/service.c
    src/structures/span.c
    src/structures/trace.c
    src/structures/counters.c
)

# Set include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find Python interpreter and libraries
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Add the shared library
add_library(otelanalyzer MODULE ${SOURCE_FILES})

# Add compile and link flags
target_compile_options(otelanalyzer PRIVATE -std=c2x)
target_link_libraries(otelanalyzer PRIVATE Python3::Python)