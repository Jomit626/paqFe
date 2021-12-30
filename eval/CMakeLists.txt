if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_EVAL)
  
  function(add_eval_target NAME)

  add_executable(${NAME}-eval eval/src/${NAME}.cpp eval/src/main.cpp)
  target_link_libraries(${NAME}-eval PUBLIC paqFe)

  endfunction(add_eval_target NAME)

  add_eval_target(orders)
endif()