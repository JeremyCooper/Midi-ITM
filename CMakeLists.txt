function(use_block_syphon)

  # Source Files
  list(APPEND SRC_FILES
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonClient.mm
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonServer.mm
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonServerDirectory.mm
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/lib/SyphonNameboundClient.mm
  )

  # Header Files
  list(APPEND HDR_FILES
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/cinderSyphon.h
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonClient.h
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonServer.h
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src/syphonServerDirectory.h
      ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/lib/SyphonNameboundClient.h
  )

  # Include Files
  
      include_directories(
              ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/src
              ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/lib
      )

  
if(APPLE)
	FIND_LIBRARY(SYPHON_LIBRARY Syphon)# ${CMAKE_SOURCE_DIR}/../../../builds/cinder/blocks/Cinder-Syphon/lib/Syphon.framework)
    list(APPEND EXTRA_LIBS ${SYPHON_LIBRARY})
endif()


  set(HDR_FILES ${HDR_FILES} PARENT_SCOPE)
  set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)
  set(EXTRA_LIBS ${EXTRA_LIBS} PARENT_SCOPE)
endfunction(use_block_syphon)

cmake_minimum_required (VERSION 3.0 FATAL_ERROR)
set(CMAKE_VERBOSE_MAKEFILE ON)
project (Midi-Itm)
use_block_syphon()
set (Current_Programming_Task \"scene creation\")

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
find_library(CF CoreFoundation)
#include_directories(${ECL_INCLUDE_DIR} ${RTMIDI_INCLUDE_DIR})
#add_executable ( Midi-Itm head/main.cpp )

set (LIBS ${ECL_LIBRARIES} ${RTMIDI_LIBRARIES} ${CF_LIBRARIES})
set (INCLUDES ${ECL_INCLUDE_DIR} ${RTMIDI_INCLUDE_DIR} ${CF_INCLUDE_DIR})
#target_link_libraries( Midi-Itm ${LIBS})

get_filename_component(CINDER_PATH "/Users/jeremycooper/builds/cinder" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

include_directories(${APP_PATH}/Scenes)
list(APPEND SCENES
		${APP_PATH}/graphics/Scenes/FlowerScene.cpp
        ${APP_PATH}/graphics/Scenes/WindowScene.cpp
        ${APP_PATH}/graphics/Scenes/GLScene.cpp
		${APP_PATH}/graphics/Scenes/ThrowbackScene.cpp
		)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")
ci_make_app(
	APP_NAME	"Midi-Itm"
	CINDER_PATH	${CINDER_PATH}
	SOURCES
		${SRC_FILES}
		${APP_PATH}/head/main.cpp
		${APP_PATH}/graphics/Graphics.cpp
		${APP_PATH}/graphics/Layer.cpp
		${APP_PATH}/graphics/Scene.cpp
		${APP_PATH}/graphics/SceneLoader.cpp
		${SCENES}
	INCLUDES	${INCLUDES}
	LIBRARIES
        ${LIBS}
        ${EXTRA_LIBS}
)


#install (TARGETS Midi-Itm DESTINATION ${BIN_DIR})