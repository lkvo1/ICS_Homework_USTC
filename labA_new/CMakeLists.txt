cmake_minimum_required(VERSION 3.15)

project(lc3-assembler)

set(CMAKE_CXX_STANDARD 11)

option(USE_CPP "Add #define USE_CPP at beginning to indicate usage of C++." ON)

if (USE_CPP)
    add_compile_definitions(USE_CPP)
endif()

add_library(assembler
    src/tokenizer.cpp
    src/instruction.cpp
    src/parser.cpp
    src/asmFmt.cpp
    # src/operand.cpp
    # src/instruction.cpp
    # src/assembler.cpp
)

target_include_directories(assembler PUBLIC include)

add_executable(lc3-assembler src/main.cpp)

target_link_libraries(lc3-assembler PRIVATE assembler)

enable_testing()

# include(AddGoogleTest)
# add_subdirectory(unittest)
# add_subdirectory(test)