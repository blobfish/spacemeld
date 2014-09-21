/*
SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com

This file is part of SpaceMeld.

SpaceMeld is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SpaceMeld is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtCore/QSettings>
#include <QtCore/QDebug>
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
