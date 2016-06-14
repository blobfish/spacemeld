#SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
#Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com
#
#This file is part of SpaceMeld.
#
#SpaceMeld is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#SpaceMeld is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#there is a slight flaw in the following. User can pass in a
#variable and force the building of the wrong platform.
#example: On windows "cmake -DBUILD_EXPORT_X11_MAG ../source"
#will force the building of the x11 window system interface. Error!
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

add_subdirectory(spacemeldtests)
