add_library(peer_lib)

target_sources(peer_lib
  PRIVATE
  Peer.cpp)

target_include_directories(peer_lib
  PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIR}>)
