cmake_minimum_required(VERSION 3.14)
project(chip8)
set(CMAKE_CXX_STANDARD 20 CMAKE_COMPILE_WARNING_AS_ERROR ON)
include_directories(include)

find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(
    chip8
    #src/main.cpp
    src/chip8.cpp
    src/opparser.cpp
    src/ParsedOpResults.cpp
    src/RegValue.cpp
    src/SdlDrawer.cpp
    #src/sdl_test.cpp
    test/test_chip8.cpp
    test/test_op_parser.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

target_link_libraries(
    chip8
    gtest
    gtest_main
    gmock_main
    SDL2::SDL2
)

include(GoogleTest)
gtest_discover_tests(chip8)
