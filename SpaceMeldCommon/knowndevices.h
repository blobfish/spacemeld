#ifndef KNOWNDEVICES_H
#define KNOWNDEVICES_H

#include <QVector>
#include "deviceinfo.h"

class KnownDevices
{
public:
    KnownDevices();
    DeviceInfo matchByKey(const QString &string);
    bool settingsHelper(DeviceInfo &other);

private:
    void createKnownList();
    DeviceInfos devices;    
};

#endif // KNOWNDEVICES_H
