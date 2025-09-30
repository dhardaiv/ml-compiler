cmake_minimum_required(VERSION 3.10)
project(ToyMLCompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(toy-ml-compiler
    src/main.cpp
    src/codegen_ir.cpp
    src/codegen_c.cpp
    src/runtime.cpp
)

target_include_directories(toy-ml-compiler PRIVATE include)
target_link_libraries(toy-ml-compiler LLVM)
