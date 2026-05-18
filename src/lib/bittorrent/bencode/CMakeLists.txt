# Create the target
add_library(bencode)

target_sources(bencode
  PRIVATE
  BencodeDecoder.cpp
  TorrentManager.cpp
  TorrentMetaInfo.cpp)

target_include_directories(bencode
  PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIR}>)
