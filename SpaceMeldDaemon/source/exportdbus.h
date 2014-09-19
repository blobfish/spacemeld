#ifndef EXPORTDBUS_H
#define EXPORTDBUS_H

#ifdef SPACEMELD_BUILD_EXPORT_DBUS
#include <exportbase.h>

class DeviceBase;

class ExportDBus : public ExportBase
{
    Q_OBJECT
public:
    virtual ~ExportDBus(){}
    virtual bool initialize() override;
    static ExportDBus* instance();
private:
    explicit ExportDBus(QObject *parent = 0);
};
#endif //SPACEMELD_BUILD_EXPORT_DBUS

#endif // EXPORTDBUS_H
