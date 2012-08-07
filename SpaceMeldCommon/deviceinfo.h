#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>
#include <QStringList>

class QSettings;

namespace SpaceModelType
{
enum SpaceModel {UNKNOWN, BALL_1003, BALL_2003B, BALL_2003C, BALL_3003C, BALL_4000FLX, BALL_5000FLX,
                 MOUSE_CLASSIC, MOUSE_PLUS, MOUSE_PLUS_XT};
}

namespace ConnectionInterfaceType
{
enum ConnectionInterface {UNKNOWN, USB, SERIAL, JOYSTICK};
}

namespace OutputType
{
enum Output {UNKNOWN, X11, DBUS};

inline const char* getString(OutputType::Output index)
{
    switch(index)
    {
    case 0:
        break;
    case 1:
        return "X11";
    case 2:
        return "DBUS";
    }
    return "Unknown";
}
}

class DeviceInfo
{
public:
    DeviceInfo();
    DeviceInfo(QSettings &settings);
    bool isRecognized(){return modelId != SpaceModelType::UNKNOWN;}
    void clear();
    bool isEqual(const DeviceInfo &other) const;
    void writeSettings(QSettings &settings) const;

    SpaceModelType::SpaceModel modelId;
    ConnectionInterfaceType::ConnectionInterface interfaceId;
    QString modelName;
    QString versionString;
    QStringList matchKeys;
    QString path;
    bool enabled;
    bool detected;
    OutputType::Output output;
    qint16 maxDisplacement;//positive number. This is for client convenience only. Driver will not check or adjust.
    qint8 buttonCount;//0 through buttonCount-1
};

typedef QVector<DeviceInfo> DeviceInfos;
void dumpInfos(const DeviceInfos &infos);

#endif // DEVICEINFO_H
