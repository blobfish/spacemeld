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
