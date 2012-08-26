/*
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
