/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c DBusBaseClient -p dbusbaseclient.h:dbusbaseclient.cpp dbusbase.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "dbusbaseclient.h"

/*
 * Implementation of interface class DBusBaseClient
 */

DBusBaseClient::DBusBaseClient(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

DBusBaseClient::~DBusBaseClient()
{
}

