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

#ifndef DEVICESERIAL_H
#define DEVICESERIAL_H

#include <QtAddOnSerialPort/serialportinfo.h>
#include <QtAddOnSerialPort/serialport.h>

using namespace QtAddOn::SerialPort;

#include "devicebase.h"

class DeviceSerial : public DeviceBase
{
    Q_OBJECT
protected slots:
    void readSlot();
protected:
    explicit DeviceSerial(QObject *parent, const DeviceInfo &deviceInfoIn, const SerialPortInfo &portInfoIn);
    virtual void processPacket(const QByteArray &packet) = 0;
    SerialPortInfo portInfo;
    SerialPort *port;
};

#endif // DEVICESERIAL_H
