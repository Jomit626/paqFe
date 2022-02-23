if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_HASH)
  #find_package(Boost REQUIRED COMPONENTS Random)

  add_executable(hash-trace hash/src/trace_gen.cpp)
  target_link_libraries(hash-trace PUBLIC paqFe)

  add_executable(hash-train hash/src/main.cpp)
  target_link_libraries(hash-trace PUBLIC paqFe)
  # target_include_directories(hash-train PUBLIC ${Boost_INCLUDE_DIRS})
endif()