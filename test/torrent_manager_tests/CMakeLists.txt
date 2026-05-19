enable_testing()

add_executable( torrent_manager_tests 
  torrent_manager_tests.cpp
  )

target_link_libraries( torrent_manager_tests 
  PRIVATE
    bencode
    GTest::gtest_main
  )

include(GoogleTest)
gtest_discover_tests(torrent_manager_tests)
