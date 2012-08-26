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

#ifndef INTERFACESERIAL_H
#define INTERFACESERIAL_H

#include <QTextStream>
#include <QMap>
#include <QtAddOnSerialPort/serialportinfo.h>
#include <QtAddOnSerialPort/serialport.h>
#include "interfacebase.h"

typedef DeviceBase* (*createFunction)(QObject *parent, const DeviceInfo &infoIn,
                                      const QtAddOn::SerialPort::SerialPortInfo &portInfoIn);

typedef QMap<SpaceModelType::SpaceModel, createFunction> DeviceMap;

class InterfaceSerial : public InterfaceBase
{
    Q_OBJECT
public:
    explicit InterfaceSerial(QObject *parent, bool enabledIn);
private:
    void dumpPortInfo();
    void setupMap();
    void detect();
    bool detectBall(QtAddOn::SerialPort::SerialPort &aPort, const QtAddOn::SerialPort::SerialPortInfo &info);
    bool detectMagellan(QtAddOn::SerialPort::SerialPort &aPort, const QtAddOn::SerialPort::SerialPortInfo &info);
    DeviceBase* createDevice(QObject *parent, const DeviceInfo &infoIn,
                             const QtAddOn::SerialPort::SerialPortInfo &portInfoIn);

    QString stats;
    QTextStream statStream;
    DeviceMap deviceMap;
};

#endif // INTERFACESERIAL_H
