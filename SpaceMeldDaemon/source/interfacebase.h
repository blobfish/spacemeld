#ifndef INTERFACEBASE_H
#define INTERFACEBASE_H

#include <QObject>

#include "devicebase.h"

class InterfaceBase : public QObject
{
    Q_OBJECT
public:
    Devices getDevices(){return devices;}
    bool isEnabled(){return enabled;}
protected:
    explicit InterfaceBase(QObject *parent, bool enabledIn);

    bool enabled;
    Devices devices;
};

#endif // INTERFACEBASE_H
