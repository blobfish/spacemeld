#include <QTabWidget>
#include <QSettings>
#include <QCloseEvent>
#include "mainwindow.h"
#include "definitions.h"
#include "servicetab.h"
#include "devicetab.h"
#include "interfacetab.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("SpaceMeld Controller");
    tab = new QTabWidget(this);

    ServiceGui::Tab *serviceTab = new ServiceGui::Tab(this);
    tab->addTab(serviceTab, "Service");

    InterfaceGui::Tab *interfaceTab = new InterfaceGui::Tab(this);
    tab->addTab(interfaceTab, "Interfaces");

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
