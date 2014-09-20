cmake_minimum_required(VERSION 2.8.6) #for automoc
project(spacemelddaemon)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_AUTOMOC TRUE)

set(ADDITIONAL_INCLUDE_PATHS
    ${CMAKE_CURRENT_BINARY_DIR} #needed to find moc files.
    ${CMAKE_SOURCE_DIR}/qtsolutions/qtservice/src
    ${CMAKE_SOURCE_DIR}/spacemeldcommon
    )
INCLUDE_DIRECTORIES(${ADDITIONAL_INCLUDE_PATHS})

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
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceconfig.cpp
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/deviceinfo.cpp
      ${CMAKE_SOURCE_DIR}/spacemeldcommon/knowndevices.cpp
   )

if(BUILD_EXPORT_X11_MAG)
  add_definitions(-DSPACEMELD_BUILD_EXPORT_X11_MAG)
  set(DAEMON_X11_SRCS
        exportx11.cpp
      )
  set(DAEMON_X11_LIBS
        "X11"
     )
endif()

if(BUILD_EXPORT_WIN_MAG)
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
  find_package(Qt4 COMPONENTS QtDBus REQUIRED)
  add_definitions(-DSPACEMELD_BUILD_EXPORT_DBUS)
  set(DAEMON_DBUS_SRCS
        exportdbus.cpp
        dbusbaseserver.cpp
      )
  #don't think I need the lib because part of qt.
endif()

#this is down here after the export dbus for the "add_definitions"
find_package(Qt4 COMPONENTS QtCore QtNetwork REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(DAEMON_SRCS
      ${QTSERVICE_SRCS}
      ${DAEMON_CORE_SRCS}
      ${DAEMON_X11_SRCS}
      ${DAEMON_WIN_SRCS}
      ${DAEMON_DBUS_SRCS}
   )

set(DAEMON_LIBS
      ${QT_LIBRARIES}
      ${DAEMON_X11_LIBS}
      ${DAEMON_WIN_LIBS}
      #find_package qt4 doesn't yet support serialport. hopefully this will
      #change in near future. until then this ugly hack.
      /usr/lib/x86_64-linux-gnu/libQtSerialPort.so
   )

add_executable(spacemelddaemon ${DAEMON_SRCS})
TARGET_LINK_LIBRARIES(spacemelddaemon ${DAEMON_LIBS})
