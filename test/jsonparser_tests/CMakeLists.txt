enable_testing()

add_executable( json_parser_tests
  json_parser_tests.cpp
  )

target_link_libraries(json_parser_tests
  PRIVATE
  json_parser
  GTest::gtest_main
  )

include(GoogleTest)
gtest_discover_tests(json_parser_tests)