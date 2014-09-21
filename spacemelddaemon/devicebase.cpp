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

#include <QtCore/QDebug>

#include "devicebase.h"
#include "dbusbaseserver.h"

DeviceBase::DeviceBase(QObject *parent, const DeviceInfo &infoIn) : QObject(parent), deviceInfo(infoIn)
{

}

bool DeviceBase::exportToDBus()
{
#ifdef SPACEMELD_BUILD_EXPORT_DBUS
    QDBusConnection connection = QDBusConnection::systemBus();
    QString name = "/org/spacemeld/" + this->info().getDBUSName();

    new DBusBaseServer(this);
    if (!connection.registerObject(name, this))
    {
        qDebug() << name << " DBUS registration failed";
        return false;
    }
    else
        qDebug() << name << "DBUS registration succeeded";

    QObject::connect(this, SIGNAL(displacementOut(qint16,qint16,qint16,qint16,qint16,qint16)),
                     this, SLOT(displacementDBusConversion(qint16,qint16,qint16,qint16,qint16,qint16)));
    QObject::connect(this, SIGNAL(buttonOut(qint8,bool)), this, SLOT(buttonDBusConversion(qint8,bool)));

#endif
    return true; //everything is fine in a non-dbus environment.
}

void DeviceBase::displacementDBusConversion(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5)
{
    short temp0 = static_cast<short>(a0);
    short temp1 = static_cast<short>(a1);
    short temp2 = static_cast<short>(a2);
    short temp3 = static_cast<short>(a3);
    short temp4 = static_cast<short>(a4);
    short temp5 = static_cast<short>(a5);

    emit displacement(temp0, temp1, temp2, temp3, temp4, temp5);
}

void DeviceBase::buttonDBusConversion(qint8 buttonNumber, bool buttonDown)
{
    uchar temp = static_cast<uchar>(buttonNumber);
    emit button(temp, buttonDown);
}

uchar DeviceBase::getButtonCount()
{
    return static_cast<uchar>(deviceInfo.buttonCount);
}

QString DeviceBase::getDeviceName()
{
    return deviceInfo.modelName;
}

QString DeviceBase::getInterfaceName()
{
    return QString(ConnectionInterfaceType::getString(deviceInfo.interfaceId));
}

QString DeviceBase::getPath()
{
    return deviceInfo.path;
}
