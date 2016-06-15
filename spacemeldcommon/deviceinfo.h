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

#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <vector>
#include <string>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>

class QSettings;

namespace SpaceModelType
{
enum SpaceModel {UNKNOWN, BALL_1003, BALL_2003B, BALL_2003C, BALL_3003C, BALL_4000FLX, BALL_5000FLX,
                 MOUSE_CLASSIC, MOUSE_PLUS, MOUSE_PLUS_XT};
//strings related to this enum is provided from the knowndevices interface.
}

namespace ConnectionInterfaceType
{
enum ConnectionInterface {UNKNOWN, USB, SERIAL, JOYSTICK};
inline int size(){return 4;}
inline const char* getString(ConnectionInterfaceType::ConnectionInterface index)
{
    static const std::vector<std::string> names({"Unknown", "USB", "Serial", "Joystick"});
    return names.at(index).c_str();
}
}

namespace OutputType
{
enum Output {UNKNOWN = -1, X11, DBUS, WIN, MAC, LOCALSOCKET, SPACENAV};
inline int size(){return 6;}
inline Output getType(int index){return static_cast<Output>(index);}
inline const char* getString(int index)
{
    static const std::vector<std::string> names({"X11", "DBUS", "Win", "Mac", "Local Socket", "SpaceNav"});
    return names.at(index).c_str();
}
inline bool showAxisButtonMap(int index)
{
  if (index == 0 || index == 2 || index == 3)
    return true;
  else
    return false;
}
}

class ExportInfo
{
public:
    ExportInfo();
    ExportInfo(QSettings &settings);
    void writeSettings(QSettings &settings) const;
    void clear();

    OutputType::Output type;
    bool enabled;
    QVector<int> inverse;
    QVector<float> scale;
    QVector<int> axesMap;
    QMap<int, QString> buttonKeyMap;

};
typedef QVector<ExportInfo> ExportInfos;

class DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(QSettings &settings);
    bool isRecognized(){return modelId != SpaceModelType::UNKNOWN;}
    void clear();
    bool isEqual(const DeviceInfo &other) const;
    void writeSettings(QSettings &settings) const;
    QString getDBUSName() const;
    QMap<int, QString> getButtonKeyMap(OutputType::Output output) const;

    SpaceModelType::SpaceModel modelId;
    int runTimeId;
    ConnectionInterfaceType::ConnectionInterface interfaceId;
    QString modelName;
    QString versionString;
    QStringList matchKeys;
    QString path;
    bool enabled;
    bool detected;
    ExportInfos exports;
    qint16 maxDisplacement;//positive number. This is for client convenience only. Driver will not check or adjust.
    qint8 buttonCount;//0 through buttonCount-1
};

typedef QVector<DeviceInfo> DeviceInfos;
void dumpInfos(const DeviceInfos &infos);

#endif // DEVICEINFO_H
