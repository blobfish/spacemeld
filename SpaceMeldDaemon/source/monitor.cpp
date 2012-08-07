#include "monitor.h"

#include <QDebug>

Monitor::Monitor(QObject *parent) :
    QObject(parent)
{
}

void Monitor::displacementSlot(QVector<qint16> values)
{
    qDebug() << "Displacement: " << values;
}

void Monitor::buttonSlot(qint8 buttonNumber, bool buttonDown)
{
    qDebug() << "Button: " << buttonNumber << ((buttonDown) ? " pressed" : " released");
}
