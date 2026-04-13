add_library(node)

target_sources(node 
  PRIVATE
  NODE.cpp
  )

target_include_directories(node
  PUBLIC
  $<BUILD_INTERFACE:${INCLUDE_DIR}>)
