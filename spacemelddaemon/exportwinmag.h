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

#ifndef EXPORTWINMAG_H
#define EXPORTWINMAG_H

#if defined(SPACEMELD_BUILD_EXPORT_WIN_MAG)

#include <QtCore/QList>
#include <QtCore/QVector>
#include "exportbase.h"
#include "deviceinfo.h"

#include <qt_windows.h>

#define WIN_MAG_CLASS_NAME "MAGELLAN_3D_CONTROLLER"
#define WIN_MAG_WINDOW_NAME "Magellan 3D Controller"

class ExportWinMag : public ExportBase
{
    Q_OBJECT
public:
    virtual ~ExportWinMag(){}
    virtual bool initialize();
    static ExportWinMag* instance();
    void addWindow(HWND window){clients.push_back(window);}
    
    DWORD winEventMotion;
    DWORD winEventButtonPress;
    DWORD winEventButtonRelease;
    DWORD winEventCommand;

public slots:
    void displacementIn(qint16 a0, qint16 a1, qint16 a2, qint16 a3, qint16 a4, qint16 a5);
    void buttonMessageIn(qint8 buttonNumberIn, bool buttonDownIn);
    void keyMessageIn(const QString &keySequenceIn);

private:
    explicit ExportWinMag(QObject *parent = 0);
    void sendButtonMessage(qint8 buttonNumber, bool buttonDown);
    void sendKeyMessage(const QString &keySequenceIn);

    HINSTANCE hInstance;
    HWND hwnd;
    QList<HWND> clients;
};

#endif //SPACEMELD_BUILD_EXPORT_WIN_MAG

#endif // EXPORTWINMAG_H
