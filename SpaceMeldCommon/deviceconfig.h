#ifndef DEVICECONFIG_H
#define DEVICECONFIG_H

namespace DeviceConfig
{
DeviceInfos readConfiguredDevices();
void clearConfiguredDevices();
void writeConfiguredDevices(const DeviceInfos &deviceInfos);
}

#endif // RECONCILEDEVICE_H
