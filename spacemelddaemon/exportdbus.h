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

#ifndef EXPORTDBUS_H
#define EXPORTDBUS_H

#ifdef SPACEMELD_BUILD_EXPORT_DBUS
#include "exportbase.h"

class DeviceBase;

class ExportDBus : public ExportBase
{
    Q_OBJECT
public:
    virtual ~ExportDBus(){}
    virtual bool initialize() override;
    static ExportDBus* instance();
private:
    explicit ExportDBus(QObject *parent = 0);
};
#endif //SPACEMELD_BUILD_EXPORT_DBUS

#endif // EXPORTDBUS_H
