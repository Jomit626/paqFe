if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_VERIFY)

function(add_verify_db DB_NAME)

  add_executable(${DB_NAME}-db verify/src/main.cpp verify/src/${DB_NAME}.cpp)
  target_link_libraries(${DB_NAME}-db PUBLIC paqFe)

endfunction(add_verify_db name)

add_verify_db(order1)
add_verify_db(order1ctx)
add_verify_db(coder)

endif()