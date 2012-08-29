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

#include "deviceserialball4000flx.h"

#include <QDebug>

DeviceBase* createSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn)
{
    return new DeviceSerialBall4000FLX(parent, infoIn, portInfoIn);
}

DeviceSerialBall4000FLX::DeviceSerialBall4000FLX
(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn) : DeviceSerialBall(parent, infoIn, portInfoIn)
{

}

bool DeviceSerialBall4000FLX::launch()
{
    if (!port->open(QIODevice::ReadWrite))
        return false;
    if (!setPort(*port))
        return false;
    if (!initialize())
        return false;
    connect(port, SIGNAL(readyRead()), this, SLOT(readSlot()));

    return true;
}

bool DeviceSerialBall4000FLX::initialize()
{
    // for 4000:
    // M tells ball to send movement packets
    // YC tell ball to enable cubic sensitivity. //linear would be YS
    // AD disables auto zero. To enable is AE.
    // BcCcC is a patterned beep.
    port->write("M\rYC\rAD\rBcCcC\r");

    if (port->error() != SerialPort::NoError)
    {
        qDebug() << port->error() << ": in DeviceSerialBall::initialize";
        return false;
    }

    return true;
}

void DeviceSerialBall4000FLX::processPacket(const QByteArray &packet)
{
    QByteArray temp = cleanseEscape(packet);
    if (temp.startsWith('@'))
        qDebug() << "hardware reset";//what to do?
    if (temp.startsWith('D') && temp.size() == 15)
        processDisplacementPacket(packet);
    if (temp.startsWith('K') && temp.size() == 7)
        processButtonPacket(packet);
}

void DeviceSerialBall4000FLX::processDisplacementPacket(const QByteArray &packet)
{
    QByteArray workPacket(packet);
    //remove header and time bytes.
    workPacket.remove(0, 3);

    QVector<qint16> values;
    qint16 temp;

    for (int index(0); index < workPacket.size() - 1; index += 2)
    {
        temp = ((workPacket.at(index)) << 8) | (workPacket.at(index + 1));
        values.push_back(temp);
    }

    emit displacementOut(values);
}

void DeviceSerialBall4000FLX::processButtonPacket(const QByteArray &packet)
{
    //button numbers are zero based so here they are 1 off of the numbers on device.
    static char oldState[2] = {0x00, 0x00};

    QByteArray workPacket(packet);
    workPacket.remove(0, 5);

    if (oldState[0] != workPacket.at(0))
    {
        if ((oldState[0] & 0x01) != (workPacket.at(0) & 0x01))
            emit buttonOut(7, static_cast<bool>(workPacket.at(0) & 0x01));
        if ((oldState[0] & 0x02) != (workPacket.at(0) & 0x02))
            emit buttonOut(8, static_cast<bool>(workPacket.at(0) & 0x02));
        if ((oldState[0] & 0x04) != (workPacket.at(0) & 0x04))
            emit buttonOut(9, static_cast<bool>(workPacket.at(0) & 0x04));//button A
        if ((oldState[0] & 0x08) != (workPacket.at(0) & 0x08))
            emit buttonOut(10, static_cast<bool>(workPacket.at(0) & 0x08));//button B
        if ((oldState[0] & 0x10) != (workPacket.at(0) & 0x10))
            emit buttonOut(11, static_cast<bool>(workPacket.at(0) & 0x10));//button C
    }

    if (oldState[1] != workPacket.at(1))
    {
        if ((oldState[1] & 0x01) != (workPacket.at(1) & 0x01))
            emit buttonOut(0, static_cast<bool>(workPacket.at(1) & 0x01));
        if ((oldState[1] & 0x02) != (workPacket.at(1) & 0x02))
            emit buttonOut(1, static_cast<bool>(workPacket.at(1) & 0x02));
        if ((oldState[1] & 0x04) != (workPacket.at(1) & 0x04))
            emit buttonOut(2, static_cast<bool>(workPacket.at(1) & 0x04));
        if ((oldState[1] & 0x08) != (workPacket.at(1) & 0x08))
            emit buttonOut(3, static_cast<bool>(workPacket.at(1) & 0x08));
        if ((oldState[1] & 0x10) != (workPacket.at(1) & 0x10))
            emit buttonOut(4, static_cast<bool>(workPacket.at(1) & 0x10));
        if ((oldState[1] & 0x20) != (workPacket.at(1) & 0x20))
            emit buttonOut(5, static_cast<bool>(workPacket.at(1) & 0x20));
        if ((oldState[1] & 0x80) != (workPacket.at(1) & 0x80))
            emit buttonOut(6, static_cast<bool>(workPacket.at(1) & 0x80));
    }

    oldState[0] = workPacket.at(0);
    oldState[1] = workPacket.at(1);
//    qDebug() << workPacket;

}
