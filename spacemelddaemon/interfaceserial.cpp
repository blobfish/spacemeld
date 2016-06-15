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
#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include "interfaceserial.h"
#include "deviceserial.h"
#include "deviceserialmagellan.h"
#include "deviceserialball.h"
#include "deviceserialball4000flx.h"
#include "knowndevices.h"


InterfaceSerial::InterfaceSerial(QObject *parent, bool enabledIn) :
    InterfaceBase(parent, enabledIn)
{
    statStream.setString(&stats);
    setupMap();
    if (enabled)
        detect();
}

void InterfaceSerial::setupMap()
{
    deviceMap.insert(SpaceModelType::BALL_4000FLX, createSerialBall4000FLX);
    deviceMap.insert(SpaceModelType::MOUSE_CLASSIC, createSerialMagellan);
    deviceMap.insert(SpaceModelType::MOUSE_PLUS_XT, createSerialMagellan);
}

void InterfaceSerial::detect()
{
//    dumpPortInfo();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo current;

    statStream << endl << "Serial Interface:";
    foreach(current, ports)
    {
        statStream << endl << "port: " << current.systemLocation() << endl;
        QSerialPort aPort(current);

        //look for space ball.
        statStream << "   ball:" << endl;
        if (!aPort.open(QIODevice::ReadWrite))
        {
            statStream << "      Open operation failed with: " << aPort.error() << endl;
            continue;
        }
        else
            statStream << "      Open operation succeeded" << endl;

        if (detectBall(aPort, current))
        {
            aPort.close();
            continue;
        }
        aPort.close();

        //look for magellan
        statStream << endl;
        statStream << endl << "   magellan:" << endl;
        if (!aPort.open(QIODevice::ReadWrite))
        {
            statStream << "      Open operation failed with: " << aPort.error() << endl;
            continue;
        }
        else
            statStream << "      Open operation succeeded" << endl;
        detectMagellan(aPort, current);
        aPort.close();
    }

    std::cout << stats.toStdString();
}

bool InterfaceSerial::detectBall(QSerialPort &aPort, const QSerialPortInfo &info)
{
    DeviceSerialBall::setPort(aPort);
    QString version = DeviceSerialBall::versionString(aPort);
    if (!version.isEmpty())
    {
        KnownDevices deviceCollection;
        DeviceInfo ballInfo = deviceCollection.matchByKey(version);
        if (ballInfo.isRecognized())
        {
            ballInfo.versionString = version;
            ballInfo.path = info.systemLocation();
            ballInfo.detected = true;
            statStream << "      Found " << ballInfo.modelName << endl;
            devices.push_back(createDevice(this, ballInfo, info));
            return true;
        }
        else
            statStream << "      No spaceball found for version string: " << version.replace('\r', ' ') << endl;
    }
    else
        statStream << "      No spaceball found. Version string is empty." << endl;
    return false;
}

bool InterfaceSerial::detectMagellan(QSerialPort &aPort, const QSerialPortInfo &info)
{
    DeviceSerialMagellan::setPort(aPort);
    QString version = DeviceSerialMagellan::versionString(aPort);
    if (!version.isEmpty())
    {
        KnownDevices deviceCollection;
        DeviceInfo ballInfo = deviceCollection.matchByKey(version);
        if (ballInfo.isRecognized())
        {
            ballInfo.versionString = version;
            ballInfo.path = info.systemLocation();
            ballInfo.detected = true;
            statStream << "      Found " << ballInfo.modelName << endl;
            devices.push_back(createDevice(this, ballInfo, info));
            return true;
        }
        else
            statStream << "      No magellan found for version string: " << version.replace('\r', ' ') << endl;
    }
    else
        statStream << "      No magellan found. Version string is empty." << endl;
    return false;
}

DeviceBase* InterfaceSerial::createDevice(QObject *parent, const DeviceInfo &infoIn, const QSerialPortInfo &portInfoIn)
{
    if (deviceMap.contains(infoIn.modelId))
        return (*(deviceMap.value(infoIn.modelId)))(parent, infoIn, portInfoIn);
    qDebug() << "create function is NULL";
    return 0;
}

void InterfaceSerial::dumpPortInfo()
{
    //system location: i.e. /dev/ttyS0
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QSerialPortInfo current;
    foreach(current, ports)
    {
        qDebug() << (current.isBusy() ? "port is busy" : "port is not busy");
        qDebug() << (current.isNull() ? "port is null" : "port is not null");
        qDebug() << (current.isValid() ? "port is valid" : "port is not valid");
        qDebug() << "manufacturer is: " << current.manufacturer();
        qDebug() << "port name is: " << current.portName();
        qDebug() << "product identifier is: " << current.productIdentifier();
        qDebug() << "standard rates: " << current.standardBaudRates();
        qDebug() << "system location is: " << current.systemLocation();
        qDebug() << "vendor identifier is: " << current.vendorIdentifier();
        qDebug();
    }
}
