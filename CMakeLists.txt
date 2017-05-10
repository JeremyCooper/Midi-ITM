cmake_minimum_required (VERSION 2.6)
project (Midi-Itm)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -Wall -O -pedantic -std=c++11")

#set (CMAKE_CXX_STANDARD 11)
#set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (Current_Programming_Task \"cmakefiles\")

configure_file (
	"${PROJECT_SOURCE_DIR}/head/midi-itmConfig.h.in"
	"${PROJECT_BINARY_DIR}/head/midi-itmConfig.h"
	)
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${PROJECT_SOURCE_DIR}/head)

find_package(ECL REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_executable ( Midi-Itm head/main.cpp )

set (MYLIB -lrtmidi -lecl)
target_link_libraries( Midi-Itm ${MYLIB})

#include_directories("${PROJECT_SOURCE_DIR}"}
#add_subdirectory (graphics)
