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

#include "quiter.h"
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

    Devices detectedDevices;

    //config file on active interfaces.
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup(INTERFACE_STRING);
    bool serialTest = settings.value(INTERFACE_STRING_SERIAL, false).toBool();
    if (serialTest)
    {
        serial = new InterfaceSerial(this->application(), true);
        detectedDevices += serial->getDevices();
    }
    settings.endGroup();

    //usb interface
    //joystick interface

    DeviceInfos configuredDeviceInfos = DeviceConfig::readConfiguredDevices();
    DeviceInfos reconciled = this->reconcile(detectedDevices, configuredDeviceInfos);
    dumpInfos(reconciled);
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(reconciled);

    Devices::const_iterator deviceIt;
    for (deviceIt = detectedDevices.begin(); deviceIt != detectedDevices.end(); ++deviceIt)
    {
        if (((*deviceIt)->info().enabled) && ((*deviceIt)->info().detected))
        {
            if ((*deviceIt)->launch())
                qDebug() << (*deviceIt)->info().modelName << " is ready";
            else
                qDebug() << (*deviceIt)->info().modelName << " is NOT ready";
        }
    }

    if (!detectedDevices.isEmpty())
    {
        ExportX11 *x11 = ExportX11::instance();
        x11->initialize();

        DeviceBase *device = detectedDevices.at(0);

        AxesMutator *mutate = new AxesMutator(x11);
        mutate->invertAxes(AxesMutator::ALL, true);
        mutate->setSensitivity(AxesMutator::ALL, 6.0);
        QObject::connect(device, SIGNAL(displacementOut(QVector<qint16>)), mutate, SLOT(displacementIn(QVector<qint16>)));

        QObject::connect(mutate, SIGNAL(displacementOut(QVector<qint16>)), x11, SLOT(displacementIn(QVector<qint16>)));
        QObject::connect(device, SIGNAL(buttonOut(qint8, bool)), x11, SLOT(buttonIn(qint8, bool)));


        //this is temp
//        Monitor *monitor = new Monitor(qobject_cast<QCoreApplication *>(this->application()));
//        QObject::connect(device, SIGNAL(displacementOut(QVector<qint16>)), monitor, SLOT(displacementSlot(QVector<qint16>)));
//        QObject::connect(device, SIGNAL(buttonOut(qint8, bool)), monitor, SLOT(buttonSlot(qint8, bool)));
    }

    //run for time. I am doing this so I don't have to kill the app. hopefully this will
    //let the serials return to their natural state. This is all temp.
//    QCoreApplication *app = qobject_cast<QCoreApplication *>(this->application());
//    Q_ASSERT(app);
//    quiter *quit = new quiter(app);
//    QTimer::singleShot(20000, quit, SLOT(timout())); // 1000 = 1 second.
//    QObject::connect(quit, SIGNAL(quitApp()), app, SLOT(quit()));


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
}

DeviceInfos SMDService::reconcile(Devices &detectedDevices, const DeviceInfos &configuredDeviceInfos)
{
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
                (*detectIt)->info().enabled = (*configuredIt).enabled;
                (*detectIt)->info().output = (*configuredIt).output;
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
        (*configuredIt).detected = false;
        outputInfos.push_back(*configuredIt);
    }

    return outputInfos;
}
