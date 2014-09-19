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

#include <QDebug>
#include "axesmutator.h"

AxesMutator::AxesMutator(QObject *parent, OutputType::Output outputIn) :
    QObject(parent), output(outputIn)
{
    inverse.clear();
    inverse.reserve(6);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);
    inverse.push_back(1);

    axesMap.clear();
    axesMap.reserve(6);
    axesMap.push_back(0);
    axesMap.push_back(1);
    axesMap.push_back(2);
    axesMap.push_back(3);
    axesMap.push_back(4);
    axesMap.push_back(5);

    sensitivity.clear();
    sensitivity.reserve(6);
    sensitivity.push_back(1.0);
    sensitivity.push_back(1.0);
    sensitivity.push_back(1.0);
    sensitivity.push_back(1.0);
    sensitivity.push_back(1.0);
    sensitivity.push_back(1.0);
}

qint16 AxesMutator::doMutate(qint16 valueIn, int index)
{
    return static_cast<qint16> (valueIn * inverse.at(index) * sensitivity.at(index));
}

void AxesMutator::displacementIn(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5)
{
    a0 = doMutate(a0, 0); if(output == OutputType::WIN) a0 = sanitizeWin(a0, 0);
    a1 = doMutate(a1, 1); if(output == OutputType::WIN) a1 = sanitizeWin(a1, 1);
    a2 = doMutate(a2, 2); if(output == OutputType::WIN) a2 = sanitizeWin(a2, 2);
    a3 = doMutate(a3, 3); if(output == OutputType::WIN) a3 = sanitizeWin(a3, 3);
    a4 = doMutate(a4, 4); if(output == OutputType::WIN) a4 = sanitizeWin(a4, 4);
    a5 = doMutate(a5, 5); if(output == OutputType::WIN) a5 = sanitizeWin(a5, 5);

    emit displacementOut(a0, a1, a2, a3, a4, a5);
}

void AxesMutator::invertAxes(AxesMutator::Axis axis, bool signal)
{
    if (axis == AxesMutator::ALL)
    {
        for (int index(0); index < inverse.size(); ++index)
            inverse.replace(index, (signal ? -1 : 1));
        return;
    }

    inverse.replace(axis, (signal ? -1 : 1));
}

void AxesMutator::setMap(const QVector<int> &mapIn)
{
    if (mapIn.size() != 6 || mapIn.count(0) + mapIn.count(1) + mapIn.count(2) + mapIn.count(3) +
            mapIn.count(4) + mapIn.count(5) != 6)
        return;

    axesMap = mapIn;
}

void AxesMutator::setSensitivity(AxesMutator::Axis axis, float scale)
{
    if (axis == AxesMutator::ALL)
    {
        for (int index(0); index < sensitivity.size(); ++index)
            sensitivity.replace(index, scale);
        return;
    }

    sensitivity.replace(axis, scale);
}

void AxesMutator::setConfig(const DeviceInfo &info)
{
    this->inverse = info.exports.at(output).inverse;
    this->sensitivity = info.exports.at(output).scale;
    this->axesMap = info.exports.at(output).axesMap;
}

qint16 AxesMutator::sanitizeWin(qint16 value, int index)
{
    //win export has the 6 axes values spanned across 2 DWORDS(32 bit). 3 axes per DWORD.
    //protocol breaks the 32 bit DWORD to:
        //first 11 bits to first axes.
        //second 11 bits to second axes.
        //last 10 bits to third axes.
    //problem occurs when the number gets big/small enough to bleed into sign bit
        //thus changing direction of movement. This becomes more apparent with a high scale factor.
    //here we are keeping the values within the range of bits.
    //x and y get 11 bits and z gets 10. use index to see what the max/min return should be.
    if (index == 2 || index ==5)
    {
        //z axes.
        if (value > 255)
            return 255;
        if (value < -256)
            return -256;
    }
    //x and y axes
    if (value > 511)
        return 511;
    if (value < -512)
        return -512;
    return value;
}

