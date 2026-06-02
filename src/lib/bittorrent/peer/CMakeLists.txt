add_library(peer)

target_sources(peer
  PRIVATE
  Peer.cpp)

target_include_directories(peer
  PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIR}>)
