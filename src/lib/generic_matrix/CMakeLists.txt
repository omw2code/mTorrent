add_library(generic_matrix INTERFACE)


target_sources( generic_matrix
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
      ${CMAKE_CURRENT_SOURCE_DIR} # this will change 
    FILES
      generic_matrix.hpp
  )

target_include_directories( generic_matrix 
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>
  )


