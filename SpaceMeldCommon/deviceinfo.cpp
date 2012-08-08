#include <QSettings>
#include <QDebug>
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
    interfaceId = static_cast<ConnectionInterfaceType::ConnectionInterface>
            (settings.value(DEVICE_INTERFACE_ID_STRING, ConnectionInterfaceType::UNKNOWN).toInt());
    path = settings.value(DEVICE_PATH_STRING, "").toString();
    enabled = settings.value(DEVICE_ENABLED_STRING, false).toBool();
    detected = settings.value(DEVICE_DETECTED_STRING, false).toBool();
    output = static_cast<OutputType::Output>(settings.value(DEVICE_OUTPUT_STRING, OutputType::UNKNOWN).toInt());
    int size(0);
    size = settings.beginReadArray(AXES_INVERSE_STRING);
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
}

void DeviceInfo::writeSettings(QSettings &settings) const
{
    settings.setValue(DEVICE_ID_STRING, modelId);
    settings.setValue(DEVICE_INTERFACE_ID_STRING, interfaceId);
    settings.setValue(DEVICE_PATH_STRING, path);
    settings.setValue(DEVICE_ENABLED_STRING, enabled);
    settings.setValue(DEVICE_DETECTED_STRING, detected);
    settings.setValue(DEVICE_OUTPUT_STRING, output);
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
}

void DeviceInfo::clear()
{
    modelId = SpaceModelType::UNKNOWN;
    interfaceId = ConnectionInterfaceType::UNKNOWN;
    modelName.clear();
    versionString.clear();
    matchKeys.clear();
    path.clear();
    enabled = false;
    detected = false;
    output = OutputType::UNKNOWN;
    maxDisplacement = 0;
    buttonCount = 0;

    inverse.clear();
    inverse.reserve(6);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);

    axesMap.clear();
    axesMap.reserve(6);
    axesMap.push_back(0);
    axesMap.push_back(1);
    axesMap.push_back(2);
    axesMap.push_back(3);
    axesMap.push_back(4);
    axesMap.push_back(5);

    scale.clear();
    scale.reserve(6);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
    scale.push_back(1.0);
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
                  "   Interface Id: " << temp.interfaceId << endl <<
                  "   Version String: " << temp.versionString.replace('\r', "\n      ") << endl <<
                  "   Match Keys: " << endl;
        for (int index(0); index < temp.matchKeys.size(); ++index)
            stream << "      " << temp.matchKeys.at(index) << endl;
        stream << "   Path: " << temp.path << endl <<
                  "   Enabled: " << temp.enabled << endl <<
                  "   Detected: " << temp.detected << endl <<
                  "   Output: " << temp.output << endl <<
                  "   Max Displacement: " << temp.maxDisplacement << endl <<
                  "   Button Count: " << temp.buttonCount << endl <<
                  "   Inverse:" << endl;
        for (int index(0); index < 6; ++index)
        {
            stream << "      " << temp.inverse.at(index) << endl;
        }
        stream << "   Scale:" << endl;
        for (int index(0); index < 6; ++index)
        {
            stream << "      " << temp.scale.at(index) << endl;
        }
        stream << "   Axes Map:" << endl;
        for (int index(0); index < 6; ++index)
        {
            stream << "      " << temp.axesMap.at(index) << endl;
        }
    }
    qDebug() << message;
}
