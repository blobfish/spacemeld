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

#ifndef EXPORTSPNAV_H
#define EXPORTSPNAV_H

#if defined SPACEMELD_BUILD_EXPORT_SPACENAV

#include <QObject>

#include "exportbase.h"

class QLocalServer;
class QLocalSocket;

class ExportSpaceNav : public ExportBase
{
    Q_OBJECT
public:
  explicit ExportSpaceNav(QObject *parent = 0);
  virtual ~ExportSpaceNav();
  virtual bool initialize() override;
  static ExportSpaceNav* instance();
  
public Q_SLOTS:
  void freshConnectionSlot();
  void disconnectionSlot();
  void displacementFromDeviceSlot(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
  void buttonFromDeviceSlot(qint8 buttonNumber, bool buttonDown);
private:
  QLocalServer *theServer;
  std::vector<QLocalSocket *> theSockets;
};

#endif //SPACEMELD_BUILD_EXPORT_SPACENAV

#endif // EXPORTSPNAV_H
