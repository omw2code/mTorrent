add_library(json_parser)

target_sources(json_parser
  PRIVATE
  JsonParser.cpp
  )

target_link_libraries(json_parser PRIVATE nlohmann_json::nlohmann_json)