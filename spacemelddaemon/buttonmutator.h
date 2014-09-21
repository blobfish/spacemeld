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

#ifndef BUTTONMUTATOR_H
#define BUTTONMUTATOR_H

#include <QtCore/QObject>
#include <QtCore/QMap>

/*! @brief Mutate spaceball button messages
 * 
 * This allows the mapping of button presses to
 * key sequences.
 */
class ButtonMutator : public QObject
{
    Q_OBJECT
public:
    explicit ButtonMutator(QObject *parentIn = 0);
    void setButtonKeyMap(const QMap<int, QString> &buttonKeyMapIn);
    
public slots:
  void buttonIn(qint8 buttonNumber, bool buttonDown);
  
signals:
  void buttonMessageOut(qint8 buttonNumber, bool buttonDown);
  void keyMessageOut(const QString &keySequence);

private:
  QMap<int, QString> buttonKeyMap;
};

#endif // BUTTONMUTATOR_H
