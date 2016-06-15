/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  <copyright holder> <email>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#if defined SPACEMELD_BUILD_EXPORT_QLOCAL

#include <assert.h>
#include <sstream>

#include <QtCore/QCoreApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QDebug>

#include "exportqlocal.h"

ExportQLocal::ExportQLocal(QObject* parent): ExportBase(parent)
{
  theSockets.clear();
  theServer = new QLocalServer(this);
  theServer->setSocketOptions(QLocalServer::WorldAccessOption);
  connect(theServer, SIGNAL(newConnection()), this, SLOT(freshConnectionSlot()));
  //on kubuntu 14.10, the following creates a local domain socket file at /var/tmp/spacemeld.root
  theServer->listen("spacemeld");
}

ExportQLocal::~ExportQLocal()
{

}

bool ExportQLocal::initialize()
{
  initializedTest = true;
  return true;
}

ExportQLocal* ExportQLocal::instance()
{
  static ExportQLocal *pointer = new ExportQLocal(QCoreApplication::instance());
  if (!pointer->initializedTest)
      pointer->initialize();
  return pointer;
}

void ExportQLocal::freshConnectionSlot()
{
  while (theServer->hasPendingConnections())
  {
    QLocalSocket *freshConnection = theServer->nextPendingConnection();
    assert(freshConnection);
    connect(freshConnection, SIGNAL(disconnected()), this, SLOT(disconnectionSlot()));
    theSockets.push_back(freshConnection);
  }
}

void ExportQLocal::disconnectionSlot()
{
  QLocalSocket *sender = dynamic_cast<QLocalSocket *>(QObject::sender());
  assert(sender);
  std::vector<QLocalSocket *>::iterator it = std::find(theSockets.begin(), theSockets.end(), sender);
  assert(it != theSockets.end());
  theSockets.erase(it);
  (*it)->deleteLater();
}

void ExportQLocal::displacementFromDeviceSlot(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5)
{
  std::ostringstream stream;
  stream << 'd' << '_' <<  a0 << '_' << a1 << '_' << a2 << '_' << a3 << '_' << a4 << '_' << a5 << std::endl;
  for (auto currentSocket : theSockets)
  {
    currentSocket->write(stream.str().c_str());
    currentSocket->flush();
  }
}

void ExportQLocal::buttonFromDeviceSlot(qint8 buttonNumber, bool buttonDown)
{
  std::ostringstream stream;
  stream << 'b' << '_' << static_cast<int>(buttonNumber) << '_' << ((buttonDown) ? "1" : "0") << std::endl;
  for (auto currentSocket : theSockets)
  {
    currentSocket->write(stream.str().c_str());
    currentSocket->flush();
  }
}


#endif //SPACEMELD_BUILD_EXPORT_QLOCAL