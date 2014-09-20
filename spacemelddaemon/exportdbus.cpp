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
