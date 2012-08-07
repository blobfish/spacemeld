#include "deviceserialball.h"

#include <QDebug>

DeviceBase* createSerialBall(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn)
{
    return new DeviceSerialBall(parent, infoIn, portInfoIn);
}

DeviceSerialBall::DeviceSerialBall(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn) :
    DeviceSerial(parent, infoIn, portInfoIn)
{
}

bool DeviceSerialBall::launch()
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

bool DeviceSerialBall::setPort(SerialPort &aPort)
{
    aPort.setRts(true);
    aPort.setDtr(true);

    aPort.setDataBits(SerialPort::Data8);

    if (aPort.error() != SerialPort::NoError)
        return false;
    return true;
}

bool DeviceSerialBall::initialize()
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

QString DeviceSerialBall::versionString(SerialPort &aPort)
{
    //if this isn't broken, don't fix it!

    QByteArray temp;
    while(aPort.waitForReadyRead(1000))
        temp += aPort.readAll();

    aPort.write("@\r");
    while(aPort.waitForReadyRead(1000))
        temp += aPort.readAll();

    aPort.write("hm\r");
    while(aPort.waitForReadyRead(1000))
        temp += aPort.readAll();

    aPort.write("\"\r");
    while(aPort.waitForReadyRead(1000))
        temp += aPort.readAll();

    QByteArray message = temp;
    message.replace('\r', '\n');
    qDebug() << message;

    return temp;
}

void DeviceSerialBall::processPacket(const QByteArray &packet)
{
    qDebug() << cleanseEscape(packet).replace('\r', '\n') << "   Packet length: " << packet.size();
//    qDebug() << "   Packet length: " << packet.size();
}

QByteArray DeviceSerialBall::cleanseEscape(QByteArray packet)
{
    static const char qReplacement = 0x11;
    static const char sReplacement = 0x13;
    static const char mReplacement = 0x0d;

    packet.replace("^Q", &qReplacement);
    packet.replace("^S", &sReplacement);
    packet.replace("^M", &mReplacement);
    packet.replace("^^", "^");

    return packet;
}
