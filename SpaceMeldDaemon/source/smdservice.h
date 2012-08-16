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

private:
    DeviceInfos reconcile(const DeviceInfos &configuredDeviceInfos);
    DeviceBase* findById(const int &findId);
    void loadAxesMutate(int deviceId);
    void loadButtonMap(int deviceId);

    InterfaceSerial *serial;
    Devices detectedDevices;
};

#endif // SMDSERVICE_H
