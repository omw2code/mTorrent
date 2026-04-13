add_library( dht )

target_sources( dht
  PRIVATE
  Dht.cpp
  RoutingTable.cpp
  )

target_link_libraries(dht PRIVATE libssl)