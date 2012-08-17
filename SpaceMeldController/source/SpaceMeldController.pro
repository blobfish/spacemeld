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
