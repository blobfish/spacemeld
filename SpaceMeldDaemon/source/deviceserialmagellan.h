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

#ifndef DEVICESERIALMAGELLAN_H
#define DEVICESERIALMAGELLAN_H

#include "deviceserial.h"

class DeviceSerialMagellan : public DeviceSerial
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    static bool setPort(SerialPort &aPort);
    static QString versionString(SerialPort &aPort);
    friend DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
protected:
    explicit DeviceSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
    void processDisplacementPacket(const QByteArray &packet);
    void processButtonPacket(const QByteArray &packet);
private:
    bool initialize();
};

DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);

#endif // DEVICESERIALMAGELLAN_H
