if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_executable(paqfe test/src/paqfe.cpp)
  target_link_libraries(paqfe PUBLIC paqFe)

  add_executable(state test/src/state.cpp)
  target_link_libraries(state PUBLIC paqFe)

  add_test(NAME StateMapCheck COMMAND state)
endif()