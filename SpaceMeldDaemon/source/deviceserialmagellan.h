#ifndef DEVICESERIALMAGELLAN_H
#define DEVICESERIALMAGELLAN_H

#include "deviceserial.h"

class DeviceSerialMagellan : public DeviceSerial
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    static bool setPort(SerialPort &aPort);
    static QString versionString(SerialPort &aPort);
    friend DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
protected:
    explicit DeviceSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
    void processDisplacementPacket(const QByteArray &packet);
    void processButtonPacket(const QByteArray &packet);
private:
    bool initialize();
};

DeviceBase* createSerialMagellan(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);

#endif // DEVICESERIALMAGELLAN_H
