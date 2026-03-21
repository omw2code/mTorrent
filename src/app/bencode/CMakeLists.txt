# Create the target
add_executable( bencode )

target_sources(bencode
  PRIVATE
  BencodeDecoder.cpp)

target_link_libraries(bencode PUBLIC include)
