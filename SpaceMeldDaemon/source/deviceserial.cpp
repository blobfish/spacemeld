#include "deviceserial.h"

#include <QDebug>

using namespace QtAddOn::SerialPort;

DeviceSerial::DeviceSerial(QObject *parent, const DeviceInfo &deviceInfoIn, const SerialPortInfo &portInfoIn) :
    DeviceBase(parent, deviceInfoIn), portInfo(portInfoIn)
{
    port = new SerialPort(portInfo, this);
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
