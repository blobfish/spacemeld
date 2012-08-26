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
