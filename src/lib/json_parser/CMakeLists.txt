add_library(json_parser)

target_sources(json_parser
  PRIVATE
  JsonParser.cpp
  )

target_link_libraries(json_parser PUBLIC nlohmann_json::nlohmann_json)

target_include_directories( json_parser 
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )