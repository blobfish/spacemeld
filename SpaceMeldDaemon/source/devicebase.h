/*
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

#include <QObject>
#include <QVector>
#include "deviceinfo.h"

class DeviceBase : public QObject
{
    Q_OBJECT
public:
    DeviceInfo& info(){return deviceInfo;}
    virtual bool launch(){return false;}

protected:
    explicit DeviceBase(QObject *parent, const DeviceInfo &infoIn);
    DeviceInfo deviceInfo;

signals:
    void displacementOut(QVector<qint16> values);
    void buttonOut(qint8 buttonNumber, bool buttonDown);
};

typedef QVector<DeviceBase *> Devices;

#endif // DEVICEBASE_H
