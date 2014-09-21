/*
SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com

This file is part of SpaceMeld.

SpaceMeld is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SpaceMeld is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QtCore/QObject>
#include <QtCore/QVector>
#include "deviceinfo.h"

#ifdef SPACEMELD_BUILD_EXPORT_DBUS
#include <QtDBus/QDBusConnection>
#endif

class DeviceBase : public QObject
{
    Q_OBJECT
public:
    DeviceInfo& info(){return deviceInfo;}
    virtual bool launch(){return false;}
    virtual bool exportToDBus();

public slots:
    //dbus exported.
    uchar getButtonCount();
    QString getDeviceName();
    QString getInterfaceName();
    QString getPath();

protected:
    explicit DeviceBase(QObject *parent, const DeviceInfo &infoIn);
    DeviceInfo deviceInfo;

protected slots:
    void displacementDBusConversion(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    void buttonDBusConversion(qint8 buttonNumber, bool buttonDown);

signals:
    void displacementOut(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    void buttonOut(qint8 buttonNumber, bool buttonDown);

    //in order for qdbusabstractadaptor autorelaysignals to work the function prototypes
    //have to match exactly. So I can either allow the dbus to force types throughout
    //app or create an extra set of signals for dbus. I chose the latter and the following are for dbus.
    void displacement(short axis0, short axis1, short axis2, short axis3, short axis4, short axis5);
    void button(uchar buttonNumber, bool buttonDown);
};

typedef QVector<DeviceBase *> Devices;

#endif // DEVICEBASE_H
