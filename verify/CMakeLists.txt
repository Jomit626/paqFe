if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_VERIFY)

add_custom_target(verify-db)

function(add_verify_db DB_NAME)

  add_executable(${DB_NAME}-db verify/src/main.cpp verify/src/${DB_NAME}.cpp)
  target_link_libraries(${DB_NAME}-db PUBLIC paqFe)

  add_custom_target(
    ${DB_NAME}-db-gen
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/verify/genDB.sh ${DB_NAME} ${CMAKE_BINARY_DIR}/${DB_NAME}-db
    DEPENDS ${DB_NAME}-db
  )
  add_dependencies(verify-db ${DB_NAME}-db-gen)

endfunction(add_verify_db name)

add_verify_db(order1)
add_verify_db(order1ctx)
add_verify_db(coder)

endif()