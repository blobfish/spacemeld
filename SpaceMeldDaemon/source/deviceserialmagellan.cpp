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

#include "deviceserialmagellan.h"

#include <QDebug>
#include <QTimer>

#define TIMEOUT 100

//magellan plus xt on windows 7(AMD64)would not initialize correctly until I put the 100ms delay
//between initialize sequence. It is possible that this may cause problems, as the launch
//function returns true before the initialize sequence has ran.

DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn)
{
    return new DeviceSerialMagellan(parent, infoIn, portInfoIn);
}

DeviceSerialMagellan::DeviceSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn) :
    DeviceSerial(parent, infoIn, portInfoIn)
{
    sequence << SerialMagellanConstants::Init <<
                SerialMagellanConstants::NullRadiusDefault <<
                SerialMagellanConstants::SensitivityDefault <<
                SerialMagellanConstants::PeriodDefault <<
                SerialMagellanConstants::ModeDefault <<
                SerialMagellanConstants::ModeCompressed <<
                SerialMagellanConstants::ShortBeep;
}

bool DeviceSerialMagellan::launch()
{
    if (!port->open(QIODevice::ReadWrite))
        return false;
    if (!setPort(*port))
        return false;
    connect(port, SIGNAL(readyRead()), this, SLOT(readSlot()));
    QTimer::singleShot(TIMEOUT, this, SLOT(initializeSlot()));
    return true;
}

bool DeviceSerialMagellan::setPort(QSerialPort &aPort)
{
    aPort.setBaudRate(QSerialPort::Baud9600);
    aPort.setDataBits(QSerialPort::Data8);
    aPort.setStopBits(QSerialPort::TwoStop);
    aPort.setFlowControl(QSerialPort::HardwareControl);
    aPort.setParity(QSerialPort::NoParity);

    aPort.setDataTerminalReady(true);
    aPort.setRequestToSend(true);

    if (aPort.error() != QSerialPort::NoError)
        return false;

    return true;
}

QString DeviceSerialMagellan::versionString(QSerialPort &aPort)
{
    QByteArray temp;
    aPort.write(SerialMagellanConstants::Init);
    while (aPort.waitForReadyRead(1000))
        temp += aPort.readAll();
    temp.clear();

    aPort.write(SerialMagellanConstants::Version);
    while (aPort.waitForReadyRead(1000))
        temp += aPort.readAll();

    return QString(temp);
}

void DeviceSerialMagellan::initializeSlot()
{
    static int count = 0;
    if (count == sequence.size())
        return;
    port->write(sequence.at(count));
    count++;
    QTimer::singleShot(TIMEOUT, this, SLOT(initializeSlot()));
}

void DeviceSerialMagellan::processPacket(const QByteArray &packet)
{
//    QByteArray temp(packet);
//    qDebug() << temp.replace('\r', ' ') << "   " << temp.size();
    if (packet.startsWith('d') && packet.size() == 15)
        processDisplacementPacket(packet);
    if (packet.startsWith('k') && packet.size() == 4)
        processButtonPacket(packet);
}

void DeviceSerialMagellan::processDisplacementPacket(const QByteArray &packet)
{
    static int firstByteParity[16]={
      0xE0, 0xA0, 0xA0, 0x60, 0xA0, 0x60, 0x60, 0xA0, 0x90, 0x50, 0x50, 0x90, 0xD0, 0x90, 0x90, 0x50
    };

    static int secondByteParity[64]={
      0x80, 0x40, 0x40, 0x80, 0x40, 0x80, 0x80, 0x40, 0x40, 0x80, 0x80, 0x40, 0x80, 0x40, 0x40, 0x80,
      0x40, 0x80, 0x80, 0x40, 0x80, 0x40, 0x40, 0x80, 0x80, 0x40, 0x40, 0x80, 0xC0, 0x80, 0x80, 0x40,
      0xC0, 0x80, 0x80, 0x40, 0x80, 0x40, 0x40, 0x80, 0x80, 0x40, 0x40, 0x80, 0x40, 0x80, 0x80, 0x40,
      0x80, 0x40, 0x40, 0x80, 0x40, 0x80, 0x80, 0x40, 0x40, 0x80, 0x80, 0x40, 0x80, 0x40, 0x00, 0x80
    };

    int index, lastBytes, offset;//, values[6];
    short int accumLast, number, accumLastAdjusted;
    accumLast = offset = 0;
    QVector<qint16> values;

    for (index=1; index<13; index+=2)
    {
        /*first byte check*/
        unsigned char low, up;
        low = (packet.at(index)) & 0x0F;
        up = (packet.at(index)) & 0xF0;
        if (up != firstByteParity[low])
        {
            qDebug() << "bad first packet";
            return;
        }

        /*second byte check*/
        low = (packet.at(index+1)) & 0x3F;
        up = (packet.at(index+1)) & 0xC0;
        if (up != secondByteParity[low])
        {
            qDebug() << "bad second packet";
            return;
        }

        number = 0x0000;
        number = (short int)((packet.at(index) << 6 & 0x03C0) | (packet.at(index + 1) & 0x3F));
        if (number>512)
            number = number - 1024;
        accumLast += number;

        if (number<0)
        {
            offset--;
            offset += (int) (number+1)/64;
        }
        else
        {
            offset += (int) number/64;
        }
//        qDebug() << "number is: " << number;
        values.push_back(number);
    }

    /*last byte of packet is a sum of 6 numbers and a factor of 64. use as a packet check.
      still not sure what the second to last byte is for.*/
    accumLastAdjusted = accumLast & 0x003F;
    accumLastAdjusted += offset;
    if (accumLastAdjusted < 0)
        accumLastAdjusted += 64;
    if (accumLastAdjusted > 63)
        accumLastAdjusted -= 64;

    lastBytes = (short int)((*(packet.data() + 14)) & 0x3F);

    if (accumLastAdjusted != lastBytes)
    {
        qDebug() << "   bad packet";
        return;
    }

//    qDebug() << values;
    emit displacementOut(values);
    return;
}

void DeviceSerialMagellan::processButtonPacket(const QByteArray &packet)
{
    //button numbers are zero based so here they are 1 off of the numbers on device.
    static char oldState[3] = {0x00, 0x00, 0x00};

    char in[3];
    in[0] = packet.at(1);
    in[1] = packet.at(2);
    in[2] = packet.at(3);

    if (in[0] != oldState[0])
    {
        if ((in[0] & 0x01) != (oldState[0] & 0x01))
            emit buttonOut(0, static_cast<bool>(in[0] & 0x01));
//            qDebug() << "button 00: " << static_cast<int>(in[0] & 0x01);
        if ((in[0] & 0x02) != (oldState[0] & 0x02))
            emit buttonOut(1, static_cast<bool>(in[0] & 0x02));
//            qDebug() << "button 01: " << static_cast<int>(in[0] & 0x02);
        if ((in[0] & 0x04) != (oldState[0] & 0x04))
            emit buttonOut(2, static_cast<bool>(in[0] & 0x04));
//            qDebug() << "button 02: " << static_cast<int>(in[0] & 0x04);
        if ((in[0] & 0x08) != (oldState[0] & 0x08))
            emit buttonOut(3, static_cast<bool>(in[0] & 0x08));
//            qDebug() << "button 03: " << static_cast<int>(in[0] & 0x08);
    }

    if (in[1] != oldState[1])
    {
        if ((in[1] & 0x01) != (oldState[1] & 0x01))
            emit buttonOut(4, static_cast<bool>(in[1] & 0x01));
//            qDebug() << "button 04: " << static_cast<int>(in[1] & 0x01);
        if ((in[1] & 0x02) != (oldState[1] & 0x02))
            emit buttonOut(5, static_cast<bool>(in[1] & 0x02));
//            qDebug() << "button 05: " << static_cast<int>(in[1] & 0x02);
        if ((in[1] & 0x04) != (oldState[1] & 0x04))
            emit buttonOut(6, static_cast<bool>(in[1] & 0x04));
//            qDebug() << "button 06: " << static_cast<int>(in[1] & 0x04);
        if ((in[1] & 0x08) != (oldState[1] & 0x08))
            emit buttonOut(7, static_cast<bool>(in[1] & 0x08));
//            qDebug() << "button 07: " << static_cast<int>(in[1] & 0x08);
    }

    /*skipping asterisk button. asterisk function come in through other packets.*/
    /*magellan plus has left and right (10, 11) buttons not magellan classic */
    /*not sure if we need to filter out lower button events for magellan classic*/

    if (in[2] != oldState[2])
    {
      //skip asterisk on magellans. in[1] & 0x01
      if ((in[2] & 0x02) != (oldState[2] & 0x02))
          emit buttonOut(8, static_cast<bool>(in[2] & 0x02));
//        qDebug() << "button 08: " << static_cast<int>(in[2] & 0x02);/*left button on magellan xt*/
      if ((in[2] & 0x04) != (oldState[2] & 0x04))
          emit buttonOut(9, static_cast<bool>(in[2] & 0x04));
//        qDebug() << "button 09: " << static_cast<int>(in[2] & 0x04);/*right button on magellan xt*/
    }

    oldState[0] = in[0];
    oldState[1] = in[1];
    oldState[2] = in[2];
}
