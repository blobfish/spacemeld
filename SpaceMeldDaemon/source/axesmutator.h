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

#ifndef AXESMUTATOR_H
#define AXESMUTATOR_H

#include <QObject>
#include <QVector>
#include "deviceinfo.h"

class AxesMutator : public QObject
{
    Q_OBJECT
public:
    enum Axis {TX, TY, TZ, RX, RY, RZ, ALL};
    explicit AxesMutator(QObject *parent, OutputType::Output outputIn);
    void invertAxes(Axis axis, bool signal);
    void setMap(const QVector<int> &mapIn);
    void setSensitivity(Axis axis, float scale);
    void setConfig(const DeviceInfo &info);
    
signals:
    void displacementOut(QVector<qint16> values);
    
public slots:
    void displacementIn(QVector<qint16> values);

private:
    qint16 sanitizeWin(qint16 value, int index);
    QVector<int> inverse;
    QVector<int> axesMap;
    QVector<float> sensitivity;
    OutputType::Output output;
};

#endif // AXESMUTATOR_H
