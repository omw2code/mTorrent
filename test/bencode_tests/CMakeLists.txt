enable_testing()

add_executable( bencode_tests 
  bencode_tests.cpp
  )

target_link_libraries( bencode_tests 
  PRIVATE
    bencode
    GTest::gtest_main
  )

include(GoogleTest)
gtest_discover_tests(bencode_tests)
