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

contains(SPACE_MELD_CONFIG, USER_BUILD_X11) {
unix {
        DEFINES += SPACEMELD_BUILD_X11
        SOURCES += exportx11.cpp
        HEADERS += exportx11.h
        LIBS += -lX11
}
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
