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

#if defined SPACEMELD_BUILD_EXPORT_SPACENAV

#include <assert.h>
#include <sstream>

#include <QtCore/QCoreApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QDebug>

#include "exportspacenav.h"

ExportSpaceNav::ExportSpaceNav(QObject* parent): ExportBase(parent)
{
  theSockets.clear();
  theServer = new QLocalServer(this);
  theServer->setSocketOptions(QLocalServer::WorldAccessOption);
  connect(theServer, SIGNAL(newConnection()), this, SLOT(freshConnectionSlot()));
  theServer->listen("/var/run/spnav.sock");
}

ExportSpaceNav::~ExportSpaceNav()
{

}

bool ExportSpaceNav::initialize()
{
  initializedTest = true;
  return true;
}

ExportSpaceNav* ExportSpaceNav::instance()
{
  static ExportSpaceNav *pointer = new ExportSpaceNav(QCoreApplication::instance());
  if (!pointer->initializedTest)
      pointer->initialize();
  return pointer;
}

void ExportSpaceNav::freshConnectionSlot()
{
  while (theServer->hasPendingConnections())
  {
    QLocalSocket *freshConnection = theServer->nextPendingConnection();
    assert(freshConnection);
    connect(freshConnection, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));
    theSockets.push_back(freshConnection);
  }
}

void ExportSpaceNav::disconnectionSlot()
{
  QLocalSocket *sender = dynamic_cast<QLocalSocket *>(QObject::sender());
  assert(sender);
  std::vector<QLocalSocket *>::iterator it = std::find(theSockets.begin(), theSockets.end(), sender);
  assert(it != theSockets.end());
  theSockets.erase(it);
  (*it)->deleteLater();
}

void ExportSpaceNav::displacementFromDeviceSlot(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5)
{
  int output[8];
  output[0] = 0;
  output[1] = static_cast<int>(a0);
  output[2] = static_cast<int>(a1);
  output[3] = static_cast<int>(a2);
  output[4] = static_cast<int>(a3);
  output[5] = static_cast<int>(a4);
  output[6] = static_cast<int>(a5);
  output[7] = 100; // this is time between events. Spacemeld not using event timing at this point.
    
  for (auto currentSocket : theSockets)
  {
    currentSocket->write(reinterpret_cast<const char*>(output), sizeof(output));
    currentSocket->flush();
  }
}

void ExportSpaceNav::buttonFromDeviceSlot(qint8 buttonNumber, bool buttonDown)
{
  int output[8];
  output[0] = (buttonDown) ? 1 : 2;
  output[1] = static_cast<int>(buttonNumber);
  output[2] = 0;
  output[3] = 0;
  output[4] = 0;
  output[5] = 0;
  output[6] = 0;
  output[7] = 0;
    
  for (auto currentSocket : theSockets)
  {
    currentSocket->write(reinterpret_cast<const char*>(output), sizeof(output));
    currentSocket->flush();
  }
}


#endif //SPACEMELD_BUILD_EXPORT_SPACENAV