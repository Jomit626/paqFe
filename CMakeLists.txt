cmake_minimum_required(VERSION 3.12)

project(
  paqFe 
  VERSION 0.0.1
  DESCRIPTION "A header-only C++ compression library using a PAQ-varient algorithm adapted for text compression and FPGA implementations."
  HOMEPAGE_URL "https://github.com/Jomit626/paqFe"
  )

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE 
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

aux_source_directory($<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include> HeaderSrcs)
target_sources(${PROJECT_NAME} PUBLIC ${HeaderSrcs})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)

  include(test/CMakeLists.txt)
endif()