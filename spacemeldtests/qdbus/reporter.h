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

#ifndef REPORTER_H
#define REPORTER_H

#include <QtCore/qobject.h>

class Reporter : public QObject
{
    Q_OBJECT
public:
    Reporter(const QString &nameIn, QObject *parent = 0);
    
public Q_SLOTS:
  void displacementSlot(short axis0, short axis1, short axis2, short axis3, short axis4, short axis5);
  void buttonSlot(uchar buttonNumber, bool buttonDown);

private:
  QString name;
};

#endif // REPORTER_H
