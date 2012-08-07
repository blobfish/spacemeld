#ifndef AXESMUTATOR_H
#define AXESMUTATOR_H

#include <QObject>
#include <QVector>

class AxesMutator : public QObject
{
    Q_OBJECT
public:
    enum Axis {TX, TY, TZ, RX, RY, RZ, ALL};
    explicit AxesMutator(QObject *parent = 0);
    void invertAxes(Axis axis, bool signal);
    void setMap(const QVector<int> &mapIn);
    void setSensitivity(Axis axis, float scale);
    
signals:
    void displacementOut(QVector<qint16> values);
    
public slots:
    void displacementIn(QVector<qint16> values);

private:
    QVector<int> inverse;
    QVector<int> axesMap;
    QVector<float> sensitivity;
};

#endif // AXESMUTATOR_H
