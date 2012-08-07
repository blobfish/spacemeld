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
}

void DeviceInfo::writeSettings(QSettings &settings) const
{
    settings.setValue(DEVICE_ID_STRING, modelId);
    settings.setValue(DEVICE_INTERFACE_ID_STRING, interfaceId);
    settings.setValue(DEVICE_PATH_STRING, path);
    settings.setValue(DEVICE_ENABLED_STRING, enabled);
    settings.setValue(DEVICE_DETECTED_STRING, detected);
    settings.setValue(DEVICE_OUTPUT_STRING, output);
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
                  "   Button Count: " << temp.buttonCount << endl << endl << endl;
    }
    qDebug() << message;
}
