if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(CTest)
  
  add_executable(paqfe test/src/paqfe.cpp)
  target_link_libraries(paqfe PUBLIC paqFe)

  add_executable(state test/src/state.cpp)
  target_link_libraries(state PUBLIC paqFe)

  add_executable(order1 test/src/order1.cpp)
  target_link_libraries(order1 PUBLIC paqFe)

  add_test(NAME Order1Check COMMAND order1)
  add_test(NAME StateMapCheck COMMAND state)
endif()