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
