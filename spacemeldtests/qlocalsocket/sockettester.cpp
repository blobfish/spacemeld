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
#include <sstream>

#include <QLocalSocket>
#include <QString>
#include <QStringList>
#include <QDebug>


#include "sockettester.h"

SocketTester::SocketTester(QObject *parentIn) : QObject(parentIn)
{
  theSocket = new QLocalSocket(parentIn);
  theSocket->connectToServer("spacemeld");
  if (!theSocket->waitForConnected())
  {
    qDebug() << "couldn't connect to server:   " << theSocket->errorString();
    return;
  }
  else
    qDebug() << "connected to server";
  QObject::connect(theSocket, SIGNAL(readyRead()), this, SLOT(readSocketSlot()));
}

void SocketTester::readSocketSlot()
{
  while(theSocket->canReadLine())
  {
    QByteArray data = theSocket->readLine();
    QString dataString(data);
    QStringList splits(dataString.split('_'));
    if (splits.size() == 7 && splits.at(0).startsWith('d'))
    {
      //displacement packet. first member of split is 'd'
      std::vector<int> values(6);
      values[0] = splits.at(1).toInt();
      values[1] = splits.at(2).toInt();
      values[2] = splits.at(3).toInt();
      values[3] = splits.at(4).toInt();
      values[4] = splits.at(5).toInt();
      values[5] = splits.at(6).toInt();
      
      std::cout << "displacement:    ";
      std::ostream_iterator<int> output(std::cout, "    ");
      std::copy(values.begin(), values.end(), output);
      std::cout << std::endl;
    }
    if (splits.size() == 3 && splits.at(0).startsWith('b'))
    {
      int buttonNumber = splits.at(1).toInt();
      bool pressedTest = splits.at(2).toInt();
      
      std::ostringstream stream;
      stream << "button " << buttonNumber << ":    " << ((pressedTest) ? ("true") : ("false")) << std::endl;
      
      std::cout << stream.str();
    }
  }
}
