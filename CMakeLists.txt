cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 17)

project(pasFe)

include_directories("../paqFe")

include_directories("src/include")

aux_source_directory("src" DIR_SRCS)
aux_source_directory("src/models" DIR_SRCS)

add_executable(paqFe ${DIR_SRCS})

target_link_libraries(paqFe PUBLIC "m")