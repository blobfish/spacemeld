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
project(spacemeldcontroller)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_AUTOMOC TRUE)

set(ADDITIONAL_INCLUDE_PATHS
    ${CMAKE_CURRENT_BINARY_DIR} #needed to find moc files.
    ${CMAKE_SOURCE_DIR}/qtsolutions/qtservice/src
    ${CMAKE_SOURCE_DIR}/spacemeldcommon
    )
INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDE_PATHS})

set(CONTROLLER_CORE_SRCS
    devicetab.cpp
    main.cpp
    mainwindow.cpp
    servicetab.cpp
    userpassworddialog.cpp
    ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceconfig.cpp
    ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceinfo.cpp
    ${CMAKE_SOURCE_DIR}/spacemeldcommon/knowndevices.cpp
   )

if(BUILD_EXPORT_X11_MAG)
  add_definitions(-DSPACEMELD_BUILD_EXPORT_X11_MAG)
endif()

if(BUILD_EXPORT_WIN_MAG)
  add_definitions(-DSPACEMELD_BUILD_EXPORT_WIN_MAG)
endif()

if(BUILD_EXPORT_DBUS)
  add_definitions(-DSPACEMELD_BUILD_EXPORT_DBUS)
endif()

find_package(Qt4 COMPONENTS QtCore QtNetwork QtGui REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(CONTROLLER_RESOURCES_IN
  image.qrc
  )
QT4_ADD_RESOURCES(CONTROLLER_RESOURCES_OUT ${CONTROLLER_RESOURCES_IN})

set(CONTROLLER_SRCS
      ${QTSERVICE_SRCS}
      ${CONTROLLER_CORE_SRCS}
      ${DAEMON_DBUS_SRCS}
      ${CONTROLLER_RESOURCES_OUT}
   )

set(CONTROLLER_LIBS
      ${QT_LIBRARIES}
   )

add_executable(spacemeldcontroller ${CONTROLLER_SRCS})
TARGET_LINK_LIBRARIES(spacemeldcontroller ${CONTROLLER_LIBS})