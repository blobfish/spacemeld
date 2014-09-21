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

#include <QtCore/QDebug>

#include "buttonmutator.h"

ButtonMutator::ButtonMutator(QObject *parentIn) :
  QObject(parentIn), buttonKeyMap()
{

}

void ButtonMutator::setButtonKeyMap(const QMap<int, QString> &buttonKeyMapIn)
{
  buttonKeyMap = buttonKeyMapIn;
}

void ButtonMutator::buttonIn(qint8 buttonNumber, bool buttonDown)
{
  if(buttonKeyMap.contains(buttonNumber))
  {
    if(!buttonDown)
      return;
    QString keySequence = buttonKeyMap.value(buttonNumber);
    emit keyMessageOut(keySequence);
  }
  else
  {
    emit buttonMessageOut(buttonNumber, buttonDown);
  }
}
