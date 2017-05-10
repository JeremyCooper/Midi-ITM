cmake_minimum_required (VERSION 2.6)
project (Midi-Itm)

set (Current_Programming_Task "cmakefiles")

configure_file (
	"${PROJECT_SOURCE_DIR}/midi-itmConfig.h.in"
	"${PROJECT_BINARY_DIR}/midi-itmConfig.h"
	)

include_directories("${PROJECT_BINARY_DIR}")

add_executable ( Midi-Itm main.cpp )

#include_directories("${PROJECT_SOURCE_DIR}"}
#add_subdirectory (graphics)
