add_library(udp_lib)

target_sources(udp_lib
  PRIVATE
  UdpClient.cpp
  )

target_include_directories(udp_lib
  PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIR}>
  )