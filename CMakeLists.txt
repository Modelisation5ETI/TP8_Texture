cmake_minimum_required( VERSION 2.6 )

project( Texture )

set( CMAKE_BUILD_TYPE Debug)

# Parallax
add_subdirectory( "${CMAKE_SOURCE_DIR}/parallax" 
  "${CMAKE_BINARY_DIR}/parallax-${CMAKE_BUILD_TYPE}" )
# Bidon
add_subdirectory( "${CMAKE_SOURCE_DIR}/bidon" 
  "${CMAKE_BINARY_DIR}/bidon-${CMAKE_BUILD_TYPE}" )
# Caillou
add_subdirectory( "${CMAKE_SOURCE_DIR}/caillou" 
  "${CMAKE_BINARY_DIR}/caillou-${CMAKE_BUILD_TYPE}" )
