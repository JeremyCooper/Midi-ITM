cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE ON)
project (Midi-Itm)
set (Current_Programming_Task \"cinder integration\")

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(BIN_DIR ${Midi-Itm_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS
	"${CMAKE_CXX_FLAGS} -Wall -O -pedantic -std=c++11")

configure_file (
	"${PROJECT_SOURCE_DIR}/head/midi-itmConfig.h.in"
	"${PROJECT_BINARY_DIR}/midi-itmConfig.h"
	)
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${PROJECT_SOURCE_DIR}/head)

find_package(ECL REQUIRED)
find_package(RTMIDI REQUIRED)
#include_directories(${ECL_INCLUDE_DIR} ${RTMIDI_INCLUDE_DIR})
#add_executable ( Midi-Itm head/main.cpp )

set (LIBS ${ECL_LIBRARIES} ${RTMIDI_LIBRARIES})
set (INCLUDES ${ECL_INCLUDE_DIR} ${RTMIDI_INCLUDE_DIR})
#target_link_libraries( Midi-Itm ${LIBS})

get_filename_component(CINDER_PATH "/Users/jeremycooper/builds/cinder" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
ci_make_app(
	APP_NAME	"Midi-Itm"
	CINDER_PATH	${CINDER_PATH}
	SOURCES		${APP_PATH}/head/main.cpp
	INCLUDES	${INCLUDES}
	LIBRARIES	${LIBS}
)


#install (TARGETS Midi-Itm DESTINATION ${BIN_DIR})

#add_subdirectory (graphics)