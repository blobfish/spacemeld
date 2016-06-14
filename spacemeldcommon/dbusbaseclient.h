/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c DBusBaseClient -p dbusbaseclient.h:dbusbaseclient.cpp dbusbase.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef DBUSBASECLIENT_H_1419020433
#define DBUSBASECLIENT_H_1419020433

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.spacemeld.device
 */
class DBusBaseClient: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.spacemeld.device"; }

public:
    DBusBaseClient(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusBaseClient();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<uchar> getButtonCount()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("getButtonCount"), argumentList);
    }

    inline QDBusPendingReply<QString> getDeviceName()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("getDeviceName"), argumentList);
    }

    inline QDBusPendingReply<QString> getInterfaceName()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("getInterfaceName"), argumentList);
    }

    inline QDBusPendingReply<QString> getPath()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("getPath"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void button(uchar buttonNumber, bool buttonDown);
    void displacement(short axis0, short axis1, short axis2, short axis3, short axis4, short axis5);
};

namespace org {
  namespace spacemeld {
    typedef ::DBusBaseClient device;
  }
}
#endif
