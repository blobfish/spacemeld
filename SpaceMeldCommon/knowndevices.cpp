#include "knowndevices.h"

//how differentiate 2 identical devices on same machine? No way right now.

KnownDevices::KnownDevices() : devices()
{
    devices.reserve(30);
    createKnownList();
}

DeviceInfo KnownDevices::matchByKey(const QString &string)
{
    //should I scan whole list and make sure only one match? not doing it now.
    DeviceInfo temp;
    DeviceInfos::const_iterator it;
    for (it = devices.begin(); it != devices.end(); ++it)
    {
        QStringList::const_iterator stringIt;
        for (stringIt = (*it).matchKeys.begin(); stringIt != (*it).matchKeys.end(); ++stringIt)
        {
            if (string.contains(*stringIt))
            {
                temp = (*it);
                break;
            }
        }
    }
    return temp;
}

bool KnownDevices::settingsHelper(DeviceInfo &other)
{
    DeviceInfos::const_iterator it;
    for (it = devices.begin(); it != devices.end(); ++it)
    {
        if ((*it).modelId == other.modelId && (*it).interfaceId == other.interfaceId)
        {
            DeviceInfo temp = *it;
            other.modelName = temp.modelName;
            other.matchKeys = temp.matchKeys;
            other.maxDisplacement = temp.maxDisplacement;
            other.buttonCount = temp.buttonCount;
            return true;
        }
    }
    return false;
}

void KnownDevices::createKnownList()
{
    //this might be read from a config/xml file someday.
    //need to verify max displacement numbers.

    //from shoogen on freecad list.
    // I'd like to use FreeCAD with an older device labled 'Plus XT seriell' on the bottom.
    // It identifies as 'v MAGELLAN Version 6.80 3Dconnexion GmbH 25/09/03'.

    //spaceball 4000 flx
    DeviceInfo temp;
    temp.modelId = SpaceModelType::BALL_4000FLX;
    temp.modelName = "SPACEBALL 4000 FLX";
    temp.interfaceId = ConnectionInterfaceType::SERIAL;
    temp.matchKeys.push_back("Spaceball 4000 FLX");
    //max displacement: device didn't seem to max out like the magellans. I got numbers as high as 14770
    //with linear sensitivity. I decide to go with the max number for a short int.
    temp.maxDisplacement = 32767; //not sure about this number.
    temp.buttonCount = 12;
    devices.push_back(temp);

    temp.clear();
    temp.modelId = SpaceModelType::MOUSE_CLASSIC;
    temp.modelName = "Magellan Classic";
    temp.interfaceId = ConnectionInterfaceType::SERIAL;
    temp.matchKeys.push_back("MAGELLAN  Version 5.79");
    temp.maxDisplacement = 512; //verified.
    temp.buttonCount = 8;
    devices.push_back(temp);

    temp.clear();
    temp.modelId = SpaceModelType::MOUSE_PLUS_XT;
    temp.modelName = "Magellan Plus XT";
    temp.interfaceId = ConnectionInterfaceType::SERIAL;
    temp.matchKeys.push_back("MAGELLAN  Version 6.60");
    temp.maxDisplacement = 512; //verified.
    temp.buttonCount = 10;
    devices.push_back(temp);
}
