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

#include <iostream>
#include <assert.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtCore/QDebug>

#include "dbusbaseclient.h"
#include "reporter.h"

int main(int argc, char *argv[])
{
  int duration = 10;
  if (argc < 2)
    std::cout << "This test runs on timer and defaults to 10 seconds." << std::endl <<
    "This timer can be set with a 2nd parameter like: 'spacemeldtestx11 20'. runs for 20 seconds." << std::endl;
  else
  {
    std::string tempString(argv[1]);
    double tempDuration = std::stod(tempString);
    if (tempDuration > 1.0)
      duration = tempDuration;
  }
  
  int dummyArgCount = 1;
  QCoreApplication app(dummyArgCount, argv);
  
  QDBusConnection connection = QDBusConnection::systemBus();
  assert(connection.isConnected());
  QDBusConnectionInterface *connectionInterface = connection.interface();
  assert(connectionInterface->isServiceRegistered("org.spacemeld"));

  QDBusInterface interface("org.spacemeld", "/org/spacemeld", "org.freedesktop.DBus.Introspectable", connection);
  QDBusMessage response = interface.call("Introspect");
  if(response.type() == QDBusMessage::ErrorMessage)
  {
    qDebug() << response.errorMessage();
    return 0;
  }
  QString responseString = response.arguments().at(0).toString(); //should only be 1 big string.
  QStringList allLines = responseString.split("\n");
  QStringList deviceBusNames;
  for(QStringList::const_iterator it = allLines.cbegin(); it != allLines.cend(); ++it)
  {
    QString workString = *it;
    workString = workString.trimmed();
    if (workString.startsWith("<node name=\"") && workString.endsWith("\"/>"))
    {
      workString.remove(0, 12);
      workString.chop(3);
      deviceBusNames.append(workString);
    }
  }
  
  std::cout << std::endl << "Devices:" << std::endl;
  for (QStringList::const_iterator it = deviceBusNames.cbegin(); it != deviceBusNames.cend(); ++it)
  {
    qDebug();
    QString devicePath = "/org/spacemeld/";
    devicePath += *it;
    QDBusInterface deviceInterface("org.spacemeld", devicePath, "org.spacemeld.device", connection);
    
    QDBusMessage deviceResponse = deviceInterface.call("getDeviceName");
    if (deviceResponse.type() == QDBusMessage::ErrorMessage)
    {
      qDebug() << deviceResponse.errorMessage();
      break;
    }
    QString deviceName = deviceResponse.arguments().at(0).toString();
    qDebug() << "device name is: " << deviceName;
    
    deviceResponse = deviceInterface.call("getInterfaceName");
    if (deviceResponse.type() == QDBusMessage::ErrorMessage)
    {
      qDebug() << deviceResponse.errorMessage();
      break;
    }
    qDebug() << "interface name is: " << deviceResponse.arguments().at(0).toString();
    
    deviceResponse = deviceInterface.call("getPath");
    if (deviceResponse.type() == QDBusMessage::ErrorMessage)
    {
      qDebug() << deviceResponse.errorMessage();
      break;
    }
    qDebug() << "path is: " << deviceResponse.arguments().at(0).toString();
    
    deviceResponse = deviceInterface.call("getButtonCount");
    if (deviceResponse.type() == QDBusMessage::ErrorMessage)
    {
      qDebug() << deviceResponse.errorMessage();
      break;
    }
    qDebug() << "button count is: " << deviceResponse.arguments().at(0).toInt();
    
    Reporter *reporter = new Reporter(deviceName, &app);
    DBusBaseClient *dbusClient = new DBusBaseClient("org.spacemeld", devicePath, connection, &app);
    assert(dbusClient->isValid());
    QObject::connect(dbusClient, SIGNAL(displacement(short, short, short, short, short, short)),
            reporter, SLOT(displacementSlot(short, short, short, short, short, short)));
    QObject::connect(dbusClient, SIGNAL(button(uchar, bool)),
            reporter, SLOT(buttonSlot(uchar, bool)));
  }
  std::cout << std::endl << "Use spaceball to test signals." << std::endl << std::endl;
  QTimer::singleShot(duration * 1000, &app, SLOT(quit()));
  app.exec();
  
  return 0;
}
