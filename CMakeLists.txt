cmake_minimum_required(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(UNIX)
  option (BUILD_EXPORT_X11_MAG "Build export interface for X11" ON)
  set(QTSERVICE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtservice_unix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtunixserversocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtunixsocket.cpp
  )
endif()
if(WIN32)
  option (BUILD_EXPORT_WIN_MAG "Build export interface for WIN" ON)
  set(QTSERVICE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtservice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qtsolutions/qtservice/src/qtservice_win.cpp
  )
endif()
option (BUILD_EXPORT_DBUS "Build export interface for DBus" ON)

#always build daemon
add_subdirectory(spacemelddaemon)

#in some cases, like dbus export only, the controller isn't needed.
#whoops this is not true. controller is needed at this point to enable interface checks
option(USER_BUILD_CONTROLLER "Build the controller application" ON)
if(USER_BUILD_CONTROLLER)
  add_subdirectory(spacemeldcontroller)
endif()
