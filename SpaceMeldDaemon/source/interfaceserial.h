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
