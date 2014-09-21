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

#include <QtCore/QCoreApplication>
#include <QtDBus/QDBusConnection>
#include <QtCore/QDebug>

#include "devicebase.h"
#include "exportdbus.h"

ExportDBus::ExportDBus(QObject *parent) : ExportBase(parent)
{
    QDBusConnection connection = QDBusConnection::systemBus();
    bool ret = connection.registerService(QString::fromLatin1("org.spacemeld"));
    if (!ret)
        qDebug() << "DBUS service register didn't work";
    else
        qDebug() << "DBUS service register worked";
}

bool ExportDBus::initialize()
{
   initializedTest = true;
   return true;
}

ExportDBus* ExportDBus::instance()
{
    static ExportDBus *pointer = new ExportDBus(QCoreApplication::instance());
    if (!pointer->initializedTest)
        pointer->initialize();
    return pointer;
}
