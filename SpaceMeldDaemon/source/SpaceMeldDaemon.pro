#   This file is part of SpaceMeld.
#
#   SpaceMeld is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   SpaceMeld is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.

#-------------------------------------------------
#
# Project created by QtCreator 2012-07-05T11:45:57
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = SpaceMeldDaemon
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += serialport

TEMPLATE = app

include(../../SpaceMeldCommon/spacemeld.pri)

contains(SPACE_MELD_CONFIG, USER_BUILD_EXPORT_X11_MAG) {
unix {
        DEFINES += SPACEMELD_BUILD_EXPORT_X11_MAG
        SOURCES += exportx11.cpp
        HEADERS += exportx11.h
        LIBS += -lX11
}
}

contains(SPACE_MELD_CONFIG, USER_BUILD_EXPORT_WIN_MAG) {
win32 {
        DEFINES += SPACEMELD_BUILD_EXPORT_WIN_MAG
        SOURCES += exportwinmag.cpp
        HEADERS += exportwinmag.h
        LIBS += -luser32 -lshell32 -lgdi32
}
}

contains(SPACE_MELD_CONFIG, USER_BUILD_EXPORT_DBUS) {
        DEFINES += SPACEMELD_BUILD_EXPORT_DBUS
        SOURCES += exportdbus.cpp dbusbaseserver.cpp
        HEADERS += exportdbus.h dbusbaseserver.h
        CONFIG += qdbus
}

SOURCES += main.cpp \
    smdservice.cpp \
    interfacebase.cpp \
    devicebase.cpp \
    deviceserial.cpp \
    interfaceserial.cpp \
    deviceserialmagellan.cpp \
    deviceserialball.cpp \
    monitor.cpp \
    deviceserialball4000flx.cpp \
    exportbase.cpp \
    axesmutator.cpp \
    ../../SpaceMeldCommon/deviceconfig.cpp \
    ../../SpaceMeldCommon/deviceinfo.cpp \
    ../../SpaceMeldCommon/knowndevices.cpp

HEADERS += \
    smdservice.h \
    interfacebase.h \
    devicebase.h \
    deviceserial.h \
    interfaceserial.h \
    deviceserialmagellan.h \
    deviceserialball.h \
    monitor.h \
    deviceserialball4000flx.h \
    exportbase.h \
    axesmutator.h \
    ../../SpaceMeldCommon/deviceconfig.h \
    ../../SpaceMeldCommon/deviceinfo.h \
    ../../SpaceMeldCommon/knowndevices.h
