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
#include <QtCore/QMapIterator>
#include <QtCore/QDebug>
#include "definitions.h"
#include "deviceinfo.h"

DeviceInfo::DeviceInfo()
{
    clear();
}

DeviceInfo::DeviceInfo(QSettings &settings)
{
    clear();
    modelId = static_cast<SpaceModelType::SpaceModel>(settings.value(DEVICE_ID_STRING, SpaceModelType::UNKNOWN).toInt());
    runTimeId = settings.value(DEVICE_RUNTIMEID_STRING, -1).toInt();
    interfaceId = static_cast<ConnectionInterfaceType::ConnectionInterface>
            (settings.value(DEVICE_INTERFACE_ID_STRING, ConnectionInterfaceType::UNKNOWN).toInt());
    path = settings.value(DEVICE_PATH_STRING, "").toString();
    enabled = settings.value(DEVICE_ENABLED_STRING, false).toBool();
    detected = settings.value(DEVICE_DETECTED_STRING, false).toBool();
    int size(0);

    size = settings.beginReadArray(DEVICE_EXPORT_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        exports[index] = ExportInfo(settings);
    }
    settings.endArray();
}

void DeviceInfo::writeSettings(QSettings &settings) const
{
    settings.setValue(DEVICE_ID_STRING, modelId);
    settings.setValue(DEVICE_RUNTIMEID_STRING, runTimeId);
    settings.setValue(DEVICE_INTERFACE_ID_STRING, interfaceId);
    settings.setValue(DEVICE_PATH_STRING, path);
    settings.setValue(DEVICE_ENABLED_STRING, enabled);
    settings.setValue(DEVICE_DETECTED_STRING, detected);

    settings.beginWriteArray(DEVICE_EXPORT_STRING);
    for (int index(0); index < exports.size(); ++index)
    {
        settings.setArrayIndex(index);
        exports.at(index).writeSettings(settings);
    }
    settings.endArray();
}

void DeviceInfo::clear()
{
    modelId = SpaceModelType::UNKNOWN;
    runTimeId = -1;
    interfaceId = ConnectionInterfaceType::UNKNOWN;
    modelName.clear();
    versionString.clear();
    matchKeys.clear();
    path.clear();
    enabled = false;
    detected = false;
    exports.clear();
    for (int index(0); index < OutputType::size(); ++index)
    {
        ExportInfo temp;
        temp.type = OutputType::getType(index);
        exports.push_back(temp);
    }
    maxDisplacement = 0;
    buttonCount = 0;
}

bool DeviceInfo::isEqual(const DeviceInfo &other) const
{
    //path is relative to serial. It might be or might not be
    //relative to other interfaces. To be determined.
    return (this->modelId == other.modelId &&
            this->interfaceId == other.interfaceId &&
            this->path == other.path);
}

void dumpInfos (const DeviceInfos &infos)
{
    QString message;
    QTextStream stream(&message);
    stream << "Devices: " << endl;
    DeviceInfos::const_iterator it;

    for (it = infos.begin(); it != infos.end(); ++it)
    {
        DeviceInfo temp = (*it);
        stream << "   Model Name: " << temp.modelName << endl <<
                  "   Model Id: " << temp.modelId << endl <<
                  "   Run Time Id: " << temp.runTimeId << endl <<
                  "   Interface Id: " << temp.interfaceId << endl <<
                  "   Version String: " << temp.versionString.replace('\r', "\n      ") << endl <<
                  "   Match Keys: " << endl;
        for (int index(0); index < temp.matchKeys.size(); ++index)
            stream << "      " << temp.matchKeys.at(index) << endl;
        stream << "   Path: " << temp.path << endl <<
                  "   Enabled: " << temp.enabled << endl <<
                  "   Detected: " << temp.detected << endl <<
                  "   Max Displacement: " << temp.maxDisplacement << endl <<
                  "   Button Count: " << temp.buttonCount << endl <<
                  "   Exports:" << endl;
        ExportInfos::const_iterator eit;
        for (eit = (*it).exports.begin(); eit != (*it).exports.end(); ++eit)
        {
            stream << "      " << OutputType::getString((*eit).type) << endl <<
                      "         Enabled: " << (((*eit).enabled) ? "True" : "False") << endl <<
                      "         Inverse:   ";
            for (int index(0); index < (*eit).inverse.size(); ++index)
                stream << (*eit).inverse.at(index) << "   ";
            stream << endl << "         Scale:   ";
            for (int index(0); index < (*eit).scale.size(); ++index)
                stream << (*eit).scale.at(index) << "   ";
            stream << endl << "         Axes Map:   ";
            for (int index(0); index < (*eit).axesMap.size(); ++index)
                stream << (*eit).axesMap.at(index) << "   ";
            stream << endl << "         Button Key Map:" << endl;
            QMapIterator<int, QString> mapIt((*eit).buttonKeyMap);
            while(mapIt.hasNext())
            {
                mapIt.next();
                stream << "            Button: " << mapIt.key() << "   Key Sequence: " << mapIt.value() << endl;
            }
        }
        stream << endl << endl;
    }
    qDebug() << message;
}

ExportInfo::ExportInfo() : type(OutputType::UNKNOWN), enabled(false)
{
    clear();
}

void ExportInfo::clear()
{
    type = OutputType::UNKNOWN;
    enabled = false;

    inverse.clear();
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);

    scale.clear();
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);

    axesMap.clear();
    axesMap.push_back(0);
    axesMap.push_back(1);
    axesMap.push_back(2);
    axesMap.push_back(3);
    axesMap.push_back(4);
    axesMap.push_back(5);

    buttonKeyMap.clear();
}

ExportInfo::ExportInfo(QSettings &settings)
{
    clear();

    type = static_cast<OutputType::Output>(settings.value(EXPORT_TYPE_STRING, OutputType::UNKNOWN).toInt());
    enabled = settings.value(EXPORT_ENABLED_STRING, false).toBool();

    int size = settings.beginReadArray(AXES_INVERSE_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        inverse[index] = settings.value(QString("%1").arg(index), 1).toInt();
    }
    settings.endArray();
    size = settings.beginReadArray(AXES_SCALE_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        scale[index] = settings.value(QString("%1").arg(index), 1.0).toFloat();
    }
    settings.endArray();
    size = settings.beginReadArray(AXES_MAP_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        axesMap[index] = settings.value(QString("%1").arg(index), index).toInt();
    }
    settings.endArray();
    size = settings.beginReadArray(BUTTON_KEY_MAP_STRING);
    for (int index(0); index < size; ++index)
    {
        settings.setArrayIndex(index);
        int button = settings.value(BUTTON_STRING, -1).toInt();
        QString temp = settings.value(KEY_STRING).toString();
        if (button != -1 && !temp.isEmpty())
            buttonKeyMap.insert(button, temp);
    }
    settings.endArray();
}

void ExportInfo::writeSettings(QSettings &settings) const
{
    settings.setValue(EXPORT_TYPE_STRING, static_cast<int>(type));
    settings.setValue(EXPORT_ENABLED_STRING, enabled);

    settings.beginWriteArray(AXES_INVERSE_STRING);
    for (int index(0); index < 6; ++index)
    {
        settings.setArrayIndex(index);
        settings.setValue(QString("%1").arg(index), inverse.at(index));
    }
    settings.endArray();

    settings.beginWriteArray(AXES_SCALE_STRING);
    for (int index(0); index < 6; ++index)
    {
        settings.setArrayIndex(index);
        settings.setValue(QString("%1").arg(index), scale.at(index));
    }
    settings.endArray();

    settings.beginWriteArray(AXES_MAP_STRING);
    for (int index(0); index < 6; ++index)
    {
        settings.setArrayIndex(index);
        settings.setValue(QString("%1").arg(index), axesMap.at(index));
    }
    settings.endArray();

    settings.beginWriteArray(BUTTON_KEY_MAP_STRING);
    QMap<int, QString>::const_iterator buttonIt;
    int count(0);
    for (buttonIt = buttonKeyMap.constBegin(); buttonIt != buttonKeyMap.constEnd(); buttonIt++)
    {
        if (buttonIt.value().isEmpty())
            continue;
        settings.setArrayIndex(count);
        settings.setValue(BUTTON_STRING, buttonIt.key());
        settings.setValue(KEY_STRING, buttonIt.value());
        count++;
    }
    settings.endArray();
}

QString DeviceInfo::getDBUSName() const
{
    //cant have any / in the dbus name.
    QString tempPath = path;
    tempPath.replace("/", "_");

    //can't have any spaces in the dbus name.
    QString tempName = modelName;
    tempName.replace(" ", "_");

    QString out;
    QTextStream stream(&out);
    stream << tempName << "_" << ConnectionInterfaceType::getString(interfaceId) << "_" << tempPath;
    return out;
}

QMap<int, QString> DeviceInfo::getButtonKeyMap(OutputType::Output output) const
{
  for (auto it = exports.begin(); it != exports.end(); ++it)
  {
    if(it->type == output)
      return it->buttonKeyMap;
  }
  qDebug() << "Error: no output type in DeviceInfo::getButtonKeyMap";
  return QMap<int, QString>();
}
