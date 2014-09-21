/*
SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com

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

#include <QtCore/qglobal.h>
#include <QtCore/QObject>
#include <QtCore/QVector>
#include "exportbase.h"
#include "deviceinfo.h"

#if defined(Q_WS_X11) && defined(SPACEMELD_BUILD_EXPORT_X11_MAG)
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class ExportX11 : public ExportBase
{
    Q_OBJECT
public:
    virtual ~ExportX11();
    virtual bool initialize();
    void setButtonMap(const DeviceInfo &info);
    static int xError(Display *aDisplay, XErrorEvent *anError);
    static int xInputOutputError(Display *aDisplay);
    static ExportX11 *instance();
    
public slots:
    void displacementIn(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    void buttonIn(qint8 buttonNumber, bool buttonDown);

private slots:
    void xEventsIn();

private:
    explicit ExportX11(QObject *parent = 0);
    void finish();
    void sendButtonMessage(qint8 buttonNumber, bool buttonDown);
    void sendKeyMessage(qint8 buttonNumber, bool buttonDown);

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
