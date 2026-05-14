add_executable( client_server_tests
  client_server_test.cpp
  )

target_link_libraries(client_server_tests
  PRIVATE
  tcp_lib
  )