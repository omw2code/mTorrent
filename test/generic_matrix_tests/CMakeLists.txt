enable_testing()

add_executable( generic_matrix_tests 
  generic_matrix_tests.cpp
  )

target_link_libraries( generic_matrix_tests 
  PRIVATE
    generic_matrix
    GTest::gtest_main
  )

include(GoogleTest)
gtest_discover_tests(generic_matrix_tests)
