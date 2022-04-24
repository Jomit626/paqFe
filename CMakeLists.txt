cmake_minimum_required(VERSION 3.12)

project(
  paqFe 
  VERSION 0.0.1
  DESCRIPTION "A header-only C++ compression library using a PAQ-varient algorithm adapted for text compression and FPGA implementations."
  HOMEPAGE_URL "https://github.com/Jomit626/paqFe"
  )

option(BUILD_HASH "Build Hash Tools for Models" on)
option(BUILD_PAQFE "Build Compressor" on)
option(BUILD_TESTING "Build test" on)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE 
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

aux_source_directory($<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include> HeaderSrcs)
target_sources(${PROJECT_NAME} PUBLIC ${HeaderSrcs})

include(test/CMakeLists.txt)
include(hash/CMakeLists.txt)
