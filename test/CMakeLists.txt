if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  include(CTest)
  
  function(add_test_target_i TEST_NAME)

  add_executable(test-${TEST_NAME} test/src/${TEST_NAME}.cpp)
  target_link_libraries(test-${TEST_NAME} PUBLIC paqFe)

  endfunction(add_test_target_i TEST_NAME)

  add_test_target_i(statemap)
  add_test_target_i(usage)
  add_test_target_i(orders)
  add_test_target_i(state)
  add_test_target_i(predictor)
  add_test_target_i(contextmap)

  add_test(NAME UsageCheck COMMAND usage)
  add_test(NAME Order1Check COMMAND orders)
  add_test(NAME StateMapCheck COMMAND state)
  add_test(NAME PreictorCheck COMMAND predictor)
endif()

if(BUILD_PAQFE)

add_executable(paqfe test/src/paqfe.cpp)
target_link_libraries(paqfe PUBLIC paqFe)

endif(BUILD_PAQFE)