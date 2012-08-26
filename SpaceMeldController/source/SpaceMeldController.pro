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
# Project created by QtCreator 2012-07-30T13:41:50
#
#-------------------------------------------------

QT       += core gui

TARGET = SpaceMeldController
TEMPLATE = app

include(../../SpaceMeldCommon/spacemeld.pri)

SOURCES += main.cpp\
    mainwindow.cpp \
    servicetab.cpp \
    devicetab.cpp \
    ../../SpaceMeldCommon/deviceconfig.cpp \
    ../../SpaceMeldCommon/deviceinfo.cpp \
    ../../SpaceMeldCommon/knowndevices.cpp \
    userpassworddialog.cpp

HEADERS  += mainwindow.h\
    servicetab.h \
    devicetab.h \
    ../../SpaceMeldCommon/deviceconfig.h \
    ../../SpaceMeldCommon/deviceinfo.h \
    ../../SpaceMeldCommon/knowndevices.h \
    userpassworddialog.h

RESOURCES += \
    image.qrc
