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

cmake_minimum_required(VERSION 2.8.6) #for automoc
project(spacemelddaemon)
message("\nConfiguring daemon")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_AUTOMOC TRUE)

set(DAEMON_CORE_SRCS
      main.cpp
      smdservice.cpp
      interfacebase.cpp
      devicebase.cpp
      deviceserial.cpp
      interfaceserial.cpp
      deviceserialmagellan.cpp
      deviceserialball.cpp
      monitor.cpp
      deviceserialball4000flx.cpp
      exportbase.cpp
      axesmutator.cpp
      buttonmutator.cpp
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceconfig.cpp
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceinfo.cpp
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/knowndevices.cpp
   )

if(BUILD_EXPORT_X11_MAG)
  find_package(X11)
  if(X11_FOUND)
    add_definitions(-DSPACEMELD_BUILD_EXPORT_X11_MAG)
    set(DAEMON_X11_SRCS
          exportx11.cpp
        )
    set(DAEMON_X11_LIBS
          ${X11_LIBRARIES}
      )
    message(STATUS "Building X11 export.")
  else(X11_FOUND)
    message(WARNING "NOT building X11 export.")
  endif(X11_FOUND)
endif(BUILD_EXPORT_X11_MAG)

if(BUILD_EXPORT_WIN_MAG)
  #add find package for win32 api. Like X11 above.
  add_definitions(-DSPACEMELD_BUILD_EXPORT_WIN_MAG)
  set(DAEMON_WIN_SRCS
        exportwinmag.cpp
      )
  set(DAEMON_WIN_LIBS
        "user32"
        "shell32"
        "gdi32"
     )
endif()

if(BUILD_EXPORT_DBUS)
  find_package(Qt5DBus)
  if(Qt5DBus_FOUND)
    add_definitions(-DSPACEMELD_BUILD_EXPORT_DBUS)
    set(DAEMON_DBUS_SRCS
          exportdbus.cpp
          dbusbaseserver.cpp
        )
    set(DAEMON_DBUS_LIBS
          Qt5::DBus
        )
    message(STATUS "Building dbus export.")
  else(Qt5DBus_FOUND)
    message(WARNING "NOT building dbus export.")
  endif(Qt5DBus_FOUND)
endif()

if(BUILD_EXPORT_QLOCAL)
  find_package(Qt5Network)
  if(Qt5Network_FOUND)
    add_definitions(-DSPACEMELD_BUILD_EXPORT_QLOCAL)
    set(DAEMON_QLOCAL_SRCS
        exportqlocal.cpp
       )
    set(DAEMON_QLOCAL_LIBS
        Qt5::Network
       )
    message(STATUS "Building QLocal export.")
  else(Qt5Network_FOUND)
    message(WARNING "Not building QLocal export.")
  endif(Qt5Network_FOUND)
endif(BUILD_EXPORT_QLOCAL)

#this is down here after the export dbus for the "add_definitions"
find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)

set(ADDITIONAL_INCLUDE_PATHS
    ${CMAKE_CURRENT_BINARY_DIR} #needed to find moc files.
    ${CMAKE_SOURCE_DIR}/qtsolutions/qtservice/src
    ${CMAKE_SOURCE_DIR}/spacemeldcommon
    ${X11_INCLUDE_DIR}
    )
INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDE_PATHS})

set(DAEMON_SRCS
      ${QTSERVICE_SRCS}
      ${DAEMON_CORE_SRCS}
      ${DAEMON_X11_SRCS}
      ${DAEMON_WIN_SRCS}
      ${DAEMON_DBUS_SRCS}
      ${DAEMON_QLOCAL_SRCS}
   )

set(DAEMON_LIBS
      Qt5::Core
      Qt5::SerialPort
      ${DAEMON_X11_LIBS}
      ${DAEMON_WIN_LIBS}
      ${DAEMON_DBUS_LIBS}
      ${DAEMON_QLOCAL_LIBS}
   )

add_executable(spacemelddaemon ${DAEMON_SRCS})
TARGET_LINK_LIBRARIES(spacemelddaemon ${DAEMON_LIBS})
