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

#ifndef DEVICESERIALMAGELLAN_H
#define DEVICESERIALMAGELLAN_H

#include "deviceserial.h"

namespace SerialMagellanConstants
{
    const char SensitivityDefault[] = "q00\r";
    const char NullRadiusDefault[] = "nM\r";
    const char NullRadiusStatus[] = "nQ\r";
    const char PeriodDefault[] = "pAA\r";
    const char ModeDefault[] = "m3\r";
    const char ModeStatus[] = "mQ\r";
    const char ShortBeep[] = "b9\r";
    const char Init[] = "\rz\rz\r";
    const char Version[] = "vQ\r";
    const char KeyboardStatus[] = "kQ\r";
    const char ModeCompressed[] = "c33\r";
    const char ExtendedDefault[] = "c33\r";
    const char ExtendedStatus[] = "cQ\r";


//    sequence.

//    InitString
//    Version
//    NullRadiusDefault
//    SensitivityDefault
//    PeriodDefault
//    ModeDefault
//    ModeCompressed
//    ShortBeep

//    1000 millisecond (second) wait between open port and first write.
//    100 millisecond wait between commands.
//    10 millisecond wait between write characters.

}

class DeviceSerialMagellan : public DeviceSerial
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    static bool setPort(QSerialPort &aPort);
    static QString versionString(QSerialPort &aPort);
    friend DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);
private slots:
    void initializeSlot();
protected:
    explicit DeviceSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);
    void processDisplacementPacket(const QByteArray &packet);
    void processButtonPacket(const QByteArray &packet);
private:
    QVector<QByteArray> sequence;
};

DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn);

#endif // DEVICESERIALMAGELLAN_H
