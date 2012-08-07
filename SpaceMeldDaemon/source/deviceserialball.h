#ifndef DEVICESERIALBALL_H
#define DEVICESERIALBALL_H

#include "deviceserial.h"

class DeviceSerialBall : public DeviceSerial
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    static bool setPort(SerialPort &aPort);
    static QString versionString(SerialPort &aPort);
    friend DeviceBase* createSerialBall(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
protected:
    explicit DeviceSerialBall(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
    QByteArray cleanseEscape(QByteArray packet);
private:
    bool initialize();
};

DeviceBase* createSerialBall(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);

#endif // DEVICESERIALBALL_H
