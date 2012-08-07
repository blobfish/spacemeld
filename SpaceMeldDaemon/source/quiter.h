#ifndef QUITER_H
#define QUITER_H

#include <QObject>

class quiter : public QObject
{
    Q_OBJECT
public:
    explicit quiter(QObject *parent = 0);
signals:
    void quitApp();
public slots:
    void timout();
};

#endif // QUITER_H
