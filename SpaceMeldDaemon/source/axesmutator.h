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
    QVector<int> inverse;
    QVector<int> axesMap;
    QVector<float> sensitivity;
    OutputType::Output output;
};

#endif // AXESMUTATOR_H
