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
    void displacementSlot(QVector<qint16> values);
    void buttonSlot(qint8 buttonNumber, bool buttonDown);
};

#endif // MONITOR_H
