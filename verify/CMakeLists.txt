if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_VERIFY)

add_custom_target(verify-db)

function(add_verify_db DB_NAME)

  add_executable(vdb-${DB_NAME} verify/src/main.cpp verify/src/${DB_NAME}.cpp)
  target_link_libraries(vdb-${DB_NAME} PUBLIC paqFe)

  add_custom_target(
    vdb-${DB_NAME}-exec
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/verify/genDB.sh ${DB_NAME} ${CMAKE_BINARY_DIR}/vdb-${DB_NAME}
    DEPENDS vdb-${DB_NAME}
  )
  add_dependencies(verify-db vdb-${DB_NAME}-exec)

endfunction(add_verify_db name)

add_verify_db(coder)
add_verify_db(stateshift)
add_verify_db(staticstatemap)
add_verify_db(ordersctx)
add_verify_db(orders)
add_verify_db(mixer-config)
add_verify_db(mixer-l1-pe-predict)
add_verify_db(mixer-l1-pe-loss)
add_verify_db(mixer-l1-pe-update)
add_verify_db(mixer-l1)
add_verify_db(mixer-l2)
add_verify_db(mixer)

add_custom_target(
  vdb-all-exec
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/verify/genDB.sh all ${CMAKE_BINARY_DIR}/paqfe
  DEPENDS paqfe
)
add_dependencies(verify-db vdb-all-exec)

endif()
