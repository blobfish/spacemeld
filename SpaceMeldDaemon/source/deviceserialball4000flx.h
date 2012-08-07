#ifndef DEVICESERIALBALL4000FLX_H
#define DEVICESERIALBALL4000FLX_H

#include "deviceserialball.h"

class DeviceSerialBall4000FLX : public DeviceSerialBall
{
    Q_OBJECT
public:
    virtual bool launch();
    virtual void processPacket(const QByteArray &packet);
    friend DeviceBase* createSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn,
                                               const SerialPortInfo &portInfoIn);

protected:
    explicit DeviceSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);
    void processDisplacementPacket(const QByteArray &packet);
    void processButtonPacket(const QByteArray &packet);

private:
    bool initialize();

};

DeviceBase* createSerialBall4000FLX(QObject *parent, const DeviceInfo &infoIn, const SerialPortInfo &portInfoIn);

#endif // DEVICESERIALBALL4000FLX_H
