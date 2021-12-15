if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(CTest)
  
  function(add_test_target_i TEST_NAME)

  add_executable(${TEST_NAME} test/src/${TEST_NAME}.cpp)
  target_link_libraries(${TEST_NAME} PUBLIC paqFe)

  endfunction(add_test_target_i TEST_NAME)

  add_test_target_i(paqfe)
  add_test_target_i(state)
  #add_test_target_i(order1)
  add_test_target_i(mixer)

  #add_test(NAME Order1Check COMMAND order1)
  add_test(NAME StateMapCheck COMMAND state)
  add_test(NAME MixerCheck COMMAND mixer)
endif()