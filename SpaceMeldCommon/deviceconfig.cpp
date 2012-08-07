#include <QSettings>
#include <QDebug>
#include "definitions.h"
#include "deviceinfo.h"
#include "knowndevices.h"
#include "deviceconfig.h"

DeviceInfos DeviceConfig::readConfiguredDevices()
{
    DeviceInfos infos;
    KnownDevices knownDevices;

    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    int size = settings.beginReadArray(DEVICES_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        DeviceInfo temp(settings);
        if (!knownDevices.settingsHelper(temp))
            qDebug() << "settings helper failed in readConfiguredDevices";
        else
            infos.push_back(temp);
    }
    settings.endArray();
    return infos;
}

void DeviceConfig::clearConfiguredDevices()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup(DEVICES_STRING);
    settings.remove("");
    settings.endGroup();
}

void DeviceConfig::writeConfiguredDevices(const DeviceInfos &deviceInfos)
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    int index(0);
    DeviceInfos::const_iterator it;
    settings.beginWriteArray(DEVICES_STRING);
    for (it = deviceInfos.begin(); it != deviceInfos.end(); ++it)
    {
        settings.setArrayIndex(index);
        (*it).writeSettings(settings);
        ++index;
    }
    settings.endArray();
}
