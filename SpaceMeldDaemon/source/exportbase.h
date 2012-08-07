#ifndef EXPORTBASE_H
#define EXPORTBASE_H

#include <QObject>

class ExportBase : public QObject
{
    Q_OBJECT
public:
    explicit ExportBase(QObject *parent = 0);
    virtual bool initialize() = 0;
    
signals:
    
public slots:
    
};

#endif // EXPORTBASE_H
