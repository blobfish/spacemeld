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

#include <QtWidgets/QTabWidget>
#include <QSettings>
#include <QCloseEvent>
#include "mainwindow.h"
#include "definitions.h"
#include "servicetab.h"
#include "devicetab.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("SpaceMeld Controller");
    tab = new QTabWidget(this);

    ServiceGui::Tab *serviceTab = new ServiceGui::Tab(this);
    tab->addTab(serviceTab, "Service");

    DeviceGui::Tab *deviceTab = new DeviceGui::Tab(this);
    tab->addTab(deviceTab, "Devices");

    connect(serviceTab, SIGNAL(driverStatus(bool)), deviceTab, SLOT(driverStatus(bool)));

    this->setCentralWidget(tab);

    restoreWindowSettings();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::saveWindowSettings()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::restoreWindowSettings()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup("MainWindow");
    if (settings.contains("size"))
        resize(settings.value("size").toSize());
    if (settings.contains("pos"))
        move(settings.value("pos").toPoint());
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
     saveWindowSettings();
     event->accept();
 }
