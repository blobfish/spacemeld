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

#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QtWidgets/QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QButtonGroup;
class QListWidget;

namespace ServiceGui
{
class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = 0);

signals:
    void driverStatus(bool);

private slots:
    void goInstallButton();
    void goUninstallButton();
    void goStartButton();
    void goStopButton();
    void goBrowse();
    void goInterfaceButton(int id);
    void goInterfaceItemChanged();

private:
    bool isServiceInstalled();
    bool isServiceRunning();
    void initialize();
    QString getServicePath();

    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QLabel *installedLabel;
    QLabel *runningLabel;

    QPushButton *installButton;
    QPushButton *uninstallButton;
    QButtonGroup *installGroup;

    QPushButton *startButton;
    QPushButton *stopButton;
    QButtonGroup *startGroup;

    QPushButton *interfaceEnableButton;
    QPushButton *interfaceDisableButton;
    QButtonGroup *interfaceButtonGroup;
    QListWidget *interfaceListWidget;
    QString interfaceSelection;
};
}

#endif // SERVICETAB_H
