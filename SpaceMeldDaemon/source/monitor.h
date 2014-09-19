/*
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

#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QVector>

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);
    
signals:
    
public slots:
    void displacementSlot(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    void buttonSlot(qint8 buttonNumber, bool buttonDown);
};

#endif // MONITOR_H
