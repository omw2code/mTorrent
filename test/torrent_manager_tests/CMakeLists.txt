enable_testing()

add_executable( torrent_manager_tests 
  torrent_manager_tests.cpp
  )

target_link_libraries( torrent_manager_tests 
  PRIVATE
    bencode
    GTest::gtest_main
  )

target_compile_definitions( torrent_manager_tests
  PRIVATE
    TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/test/torrent_manager_tests"
  )

include(GoogleTest)

gtest_discover_tests(torrent_manager_tests) 

