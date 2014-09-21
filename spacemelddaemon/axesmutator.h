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

#ifndef AXESMUTATOR_H
#define AXESMUTATOR_H

#include <QtCore/QObject>
#include <QtCore/QVector>
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
    void displacementOut(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    
public slots:
    void displacementIn(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);

private:
    qint16 sanitizeWin(qint16 value, int index);
    qint16 doMutate(qint16 valueIn, int index);
    QVector<int> inverse;
    QVector<int> axesMap;
    QVector<float> sensitivity;
    OutputType::Output output;
};

#endif // AXESMUTATOR_H
