## WARNING : Hardcoded nvidia driver

cmake_minimum_required( VERSION 2.6 )

project( Bidon )

# Configure Project
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set( SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" )
set( LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib" )
set( DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data" )
set( CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake" )
set( SHADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shader" )

configure_file( ${CMAKE_DIR}/Project_Config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/Project_Config.h @ONLY )

# find libraries                                            
find_package( OpenGL REQUIRED )

# add_executable sources
file(GLOB SRC ${SRC_DIR}/*.[ch]*)
file(GLOB LIB ${LIB_DIR}/*.[ch]*)
file(GLOB SHADER ${SHADER_DIR}/*.vert ${SHADER_DIR}/*.frag)

include_directories( ${SRC_DIR} ${LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

# Specify g++ option
add_definitions( -O2 -Wall -Wextra -std=c++11 -Wno-comment -DCORRECTION)

add_executable( ${PROJECT_NAME} ${SRC} ${LIB} ${SHADER} )
target_link_libraries( ${PROJECT_NAME} -L/usr/lib/nvidia-304/ -lm -ldl -lglut -lGLEW ${OPENGL_LIBRARIES} -pthread )
