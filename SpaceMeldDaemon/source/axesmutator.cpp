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
        valuesOut[axesMap.at(index)] = static_cast<qint16> (values.at(index) * inverse.at(index) * sensitivity.at(index));

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

