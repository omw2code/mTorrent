enable_testing()

add_executable(peer_tests
  handshake_tests.cpp
  )

target_link_libraries(peer_tests
  PRIVATE
  peer_lib
  )

include(GoogleTest)

gtest_discover_tests(peer_tests)

