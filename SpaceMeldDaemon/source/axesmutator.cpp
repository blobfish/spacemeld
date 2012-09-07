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


void AxesMutator::displacementIn(QVector<qint16> values)
{
    if (values.size() != 6)
        return;

    QVector<qint16> valuesOut;
    valuesOut.reserve(6);
    valuesOut.resize(6);

    for (int index(0); index < 6; ++index)
    {
        qint16 temp = static_cast<qint16> (values.at(index) * inverse.at(index) * sensitivity.at(index));
        if (output == OutputType::WIN)
            temp = sanitizeWin(temp, index);
        valuesOut[axesMap.at(index)] = temp;
    }

    emit displacementOut(valuesOut);
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

