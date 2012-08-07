#ifndef DEVICEBASE_H
#define DEVICEBASE_H

#include <QObject>
#include <QVector>
#include "deviceinfo.h"

class DeviceBase : public QObject
{
    Q_OBJECT
public:
    DeviceInfo& info(){return deviceInfo;}
    virtual bool launch(){return false;}

protected:
    explicit DeviceBase(QObject *parent, const DeviceInfo &infoIn);
    DeviceInfo deviceInfo;

signals:
    void displacementOut(QVector<qint16> values);
    void buttonOut(qint8 buttonNumber, bool buttonDown);
};

typedef QVector<DeviceBase *> Devices;

#endif // DEVICEBASE_H
