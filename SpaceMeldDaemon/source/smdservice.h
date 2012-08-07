#ifndef SMDSERVICE_H
#define SMDSERVICE_H

#include <qtservice.h>
#include "devicebase.h"
#include "deviceinfo.h"

class InterfaceSerial;

class SMDService : public QtService<QCoreApplication>
{
public:
    explicit SMDService(int argc, char **argv);
    ~SMDService();

protected:
    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);
    DeviceInfos reconcile(Devices &detectedDevices, const DeviceInfos &configuredDeviceInfos);

    InterfaceSerial *serial;
};

#endif // SMDSERVICE_H
