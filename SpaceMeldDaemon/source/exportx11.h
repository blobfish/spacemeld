#ifndef EXPORTX11_H
#define EXPORTX11_H

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <exportbase.h>
#include <deviceinfo.h>

#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_X11)
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class ExportX11 : public ExportBase
{
    Q_OBJECT
public:
    ~ExportX11();
    virtual bool initialize();
    void setButtonMap(const DeviceInfo &info);
    static int xError(Display *aDisplay, XErrorEvent *anError);
    static int xInputOutputError(Display *aDisplay);
    static ExportX11 *instance();
    
public slots:
    void displacementIn(QVector<qint16> values);
    void buttonIn(qint8 buttonNumber, bool buttonDown);

private slots:
    void xEventsIn();

private:
    explicit ExportX11(QObject *parent = 0);
    void finish();
    void sendButtonMessage(qint8 buttonNumber, bool buttonDown);
    void sendKeyMessage(qint8 buttonNumber, bool buttonDown);

    bool initializedTest;

    Display *display;
    Window xWindow;
    Atom xEventMotion;
    Atom xEventButtonPress;
    Atom xEventButtonRelease;
    Atom xEventCommand;

    QVector<Window> clients;
    QMap<int, QString> buttonKeyMap;
};

#endif //Q_WS_X11

#endif // EXPORTX11_H
