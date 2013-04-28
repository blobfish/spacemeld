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

#include "deviceserial.h"

#include <QDebug>

DeviceSerial::DeviceSerial(QObject *parent, const DeviceInfo &deviceInfoIn, const QSerialPortInfo &portInfoIn) :
    DeviceBase(parent, deviceInfoIn), portInfo(portInfoIn)
{
    port = new QSerialPort(portInfo, this);
}

void DeviceSerial::readSlot()
{
    static QByteArray bytesRead;
    bytesRead += port->readAll();

    if (bytesRead.endsWith('\r'))
    {
        bytesRead.chop(1);
//        qDebug() << bytesRead.replace('\r', ' ') << "   " << bytesRead.size();
        this->processPacket(bytesRead);
        bytesRead.clear();
    }
}
