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

#ifndef DEVICESERIALBALL_H
#define DEVICESERIALBALL_H

#include "deviceserial.h"

class DeviceSerialBall : public DeviceSerial
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    static bool setPort(QSerialPort &aPort);
    static QString versionString(QSerialPort &aPort);
    friend DeviceBase* createSerialBall(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);
protected:
    explicit DeviceSerialBall(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);
    QByteArray cleanseEscape(QByteArray packet);
private:
    bool initialize();
};

DeviceBase* createSerialBall(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);

#endif // DEVICESERIALBALL_H
