#include <qglobal.h>
#include <QDebug>
#include <QTimer>
#include <QSettings>
#include "definitions.h"
#include "interfaceserial.h"
#include "deviceconfig.h"
#include "smdservice.h"

#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_X11)
#include "exportx11.h"
#endif

#include "monitor.h"
#include "axesmutator.h"

SMDService::SMDService(int argc, char **argv) :
    QtService<QCoreApplication>(argc, argv, SERVICE_NAME_STRING)
{
    setServiceDescription("Driver for space mice.");
    qRegisterMetaType<QVector<qint16> >("QVector<qint16>");

    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.setValue(SERVICE_STATUS_STRING, false);
}

SMDService::~SMDService()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.setValue(SERVICE_STATUS_STRING, false);
}

void SMDService::start()
{
    logMessage("Space Meld starting");
    qDebug() << "Space Meld starting";

    //config file on active interfaces.
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup(INTERFACE_STRING);
    bool serialTest = settings.value(INTERFACE_STRING_SERIAL, false).toBool();
    settings.endGroup();
    if (serialTest)
    {
        //not sure serial needs to be member variable and it doesn't look like
        //I need the boolean in the constructor;
        serial = new InterfaceSerial(this->application(), true);
        detectedDevices += serial->getDevices();
    }

    //usb interface
    //joystick interface

    DeviceInfos configuredDeviceInfos = DeviceConfig::readConfiguredDevices();
    DeviceInfos reconciled = this->reconcile(configuredDeviceInfos);

    dumpInfos(reconciled);
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(reconciled);

    Devices::const_iterator deviceIt;
    for (deviceIt = detectedDevices.begin(); deviceIt != detectedDevices.end(); ++deviceIt)
    {
        DeviceBase *currentDevice = (*deviceIt);
        if ((currentDevice->info().enabled) && (currentDevice->info().detected))
        {
            if (currentDevice->launch())
            {
                qDebug() << currentDevice->info().modelName << " is ready";
#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_X11)
                if (currentDevice->info().output == OutputType::X11)
                {
                    ExportX11 *x11 = ExportX11::instance();

                    AxesMutator *mutate = new AxesMutator(currentDevice);
                    mutate->setObjectName("axesMutator");
                    mutate->setConfig(currentDevice->info());
                    QObject::connect(currentDevice, SIGNAL(displacementOut(QVector<qint16>)), mutate, SLOT(displacementIn(QVector<qint16>)));
                    x11->setButtonMap(currentDevice->info());

                    QObject::connect(mutate, SIGNAL(displacementOut(QVector<qint16>)), x11, SLOT(displacementIn(QVector<qint16>)));
                    QObject::connect(currentDevice, SIGNAL(buttonOut(qint8, bool)), x11, SLOT(buttonIn(qint8, bool)));

                }
#endif
            }
            else
                qDebug() << currentDevice->info().modelName << " launch FAILED";
        }
    }

        //this is temp
//        Monitor *monitor = new Monitor(qobject_cast<QCoreApplication *>(this->application()));
//        QObject::connect(device, SIGNAL(displacementOut(QVector<qint16>)), monitor, SLOT(displacementSlot(QVector<qint16>)));
//        QObject::connect(device, SIGNAL(buttonOut(qint8, bool)), monitor, SLOT(buttonSlot(qint8, bool)));

    settings.setValue(SERVICE_STATUS_STRING, true);
}

void SMDService::stop()
{
    logMessage("Space Meld stopping");
    qDebug() << "Space Meld stopping";

    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.setValue(SERVICE_STATUS_STRING, false);
}

void SMDService::pause()
{
}

void SMDService::resume()
{
}

void SMDService::processCommand(int code)
{
    //there seems to be a lag in the receiving of these commands.
    //has something to do with control focus in the controller app.
    //this might be a cause of problems of controller settings getting
    //out of sync with service.

    if (code > 99 && code < 200)
        loadAxesMutate(code);
    if (code > 199 && code < 300)
        loadButtonMap(code);

}

DeviceInfos SMDService::reconcile(const DeviceInfos &configuredDeviceInfos)
{
    int runTimeIndex(0);
    QList<DeviceInfo> copiedConfigure = configuredDeviceInfos.toList();
    DeviceInfos outputInfos;
    Devices::Iterator detectIt;
    for (detectIt = detectedDevices.begin(); detectIt != detectedDevices.end(); ++detectIt)
    {
        QList<DeviceInfo>::Iterator configuredIt;
        configuredIt = copiedConfigure.begin();
        while(configuredIt != copiedConfigure.end())
        {
            if ((*configuredIt).isEqual((*detectIt)->info()))
            {
                (*detectIt)->info().runTimeId = runTimeIndex++;
                (*detectIt)->info().enabled = (*configuredIt).enabled;
                (*detectIt)->info().output = (*configuredIt).output;
                (*detectIt)->info().inverse = (*configuredIt).inverse;
                (*detectIt)->info().scale = (*configuredIt).scale;
                (*detectIt)->info().axesMap = (*configuredIt).axesMap;
                (*detectIt)->info().buttonKeyMap = (*configuredIt).buttonKeyMap;

                copiedConfigure.erase(configuredIt);
                break;
            }
            configuredIt++;
        }
        outputInfos.push_back((*detectIt)->info());
    }

    QList<DeviceInfo>::Iterator configuredIt;
    for (configuredIt = copiedConfigure.begin(); configuredIt != copiedConfigure.end(); ++configuredIt)
    {
        (*configuredIt).runTimeId = runTimeIndex++;
        (*configuredIt).detected = false;
        outputInfos.push_back(*configuredIt);
    }

    return outputInfos;
}

DeviceBase* SMDService::findById(const int &findId)
{
    Devices::Iterator detectIt;
    for (detectIt = detectedDevices.begin(); detectIt != detectedDevices.end(); ++detectIt)
    {
        if (findId == (*detectIt)->info().runTimeId)
            return (*detectIt);
    }
    return 0;
}

void SMDService::loadAxesMutate(int deviceId)
{
    deviceId -= 100;
    DeviceBase *device = findById(deviceId);
    if (!device)
    {
        qDebug() << "couldn't find run time id in SMDService::loadAxesMutate for id: " << deviceId;
        return;
    }

    if ((!device->info().detected) || device->info().output == OutputType::DBUS)
        return;

    AxesMutator *mutate = device->findChild<AxesMutator *>("axesMutator");
    if (!mutate)
    {
        qDebug() << "couldn't find mutator in SMDService::loadAxesMutate";
        return;
    }

    DeviceInfos infos = DeviceConfig::readConfiguredDevices();
    DeviceInfos::Iterator it;
    for (it = infos.begin(); it != infos.end(); ++it)
    {
        if ((*it).runTimeId == device->info().runTimeId)
        {
            mutate->setConfig(*it);
            break;
        }
    }
}

void SMDService::loadButtonMap(int deviceId)
{
    deviceId -= 200;
    DeviceBase *device = findById(deviceId);

    if (!device)
    {
        qDebug() << "couldn't find run time id in SMDService::loadButtonMap for id: " << deviceId;
        return;
    }

    if ((!device->info().detected) || device->info().output == OutputType::DBUS)
        return;

#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_X11)
    if (device->info().output == OutputType::X11)
    {
        DeviceInfos infos = DeviceConfig::readConfiguredDevices();
        DeviceInfos::Iterator it;
        for (it = infos.begin(); it != infos.end(); ++it)
        {
            if ((*it).runTimeId == device->info().runTimeId)
            {
                ExportX11::instance()->setButtonMap(*it);
                break;
            }
        }
    }
#endif
}
