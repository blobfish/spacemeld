#include <QCoreApplication>
#include "interfaceserial.h"
#include "deviceserial.h"
#include "deviceserialmagellan.h"
#include "deviceserialball.h"
#include "deviceserialball4000flx.h"
#include "knowndevices.h"

#include <QDebug>

using namespace QtAddOn::SerialPort;

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
    QList<SerialPortInfo> ports = SerialPortInfo::availablePorts();
    SerialPortInfo current;

    foreach(current, ports)
    {
        statStream << endl << "port: " << current.systemLocation() << endl;
        SerialPort aPort(current);

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

    qDebug() << stats;
}

bool InterfaceSerial::detectBall(SerialPort &aPort, const SerialPortInfo &info)
{
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

bool InterfaceSerial::detectMagellan(SerialPort &aPort, const SerialPortInfo &info)
{
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

DeviceBase* InterfaceSerial::createDevice(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn)
{
    if (deviceMap.contains(infoIn.modelId))
        return (*(deviceMap.value(infoIn.modelId)))(parent, infoIn, portInfoIn);
    qDebug() << "create function is NULL";
    return 0;
}

void InterfaceSerial::dumpPortInfo()
{
    //system location: i.e. /dev/ttyS0
    QList<SerialPortInfo> ports = SerialPortInfo::availablePorts();
    SerialPortInfo current;
    foreach(current, ports)
    {
        qDebug() << (current.isBusy() ? "port is busy" : "port is not busy");
        qDebug() << (current.isNull() ? "port is null" : "port is not null");
        qDebug() << (current.isValid() ? "port is valid" : "port is not valid");
        qDebug() << "manufacturer is: " << current.manufacturer();
        qDebug() << "port name is: " << current.portName();
        qDebug() << "product identifier is: " << current.productIdentifier();
        qDebug() << "standard rates: " << current.standardRates();
        qDebug() << "system location is: " << current.systemLocation();
        qDebug() << "vendor identifier is: " << current.vendorIdentifier();
        qDebug();
    }
}
