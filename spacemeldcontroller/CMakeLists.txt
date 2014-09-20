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