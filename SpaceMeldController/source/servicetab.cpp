#include <QSettings>
#include <QtGui/QtGui>

#include "qtservice.h"

#include "definitions.h"
#include "servicetab.h"

using namespace ServiceGui;

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    this->setToolTip(tr("This page displays and controls the status of the driver"));
    initialize();
}

void Tab::initialize()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QString pathMessage(tr("This is the path to the service executable"));

    QHBoxLayout *pathLayout = new QHBoxLayout();
    QLabel *pathLabel = new QLabel(tr("Service Path"), this);
    pathLabel->setToolTip(pathMessage);
    pathEdit = new QLineEdit(getServicePath(), this);
    pathEdit->setToolTip(pathMessage);
    browseButton = new QPushButton(tr("Browse"), this);
    browseButton->setToolTip(pathMessage);
    connect(browseButton, SIGNAL(clicked()), this, SLOT(goBrowse()));
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathEdit);
    pathLayout->addWidget(browseButton);
    mainLayout->addLayout(pathLayout);
    mainLayout->addStretch();

    QHBoxLayout *installLayout = new QHBoxLayout();
    installedLabel = new QLabel(this);
    if (isServiceInstalled())
        installedLabel->setText(tr("Service is installed"));
    else
        installedLabel->setText(tr("Service is not installed"));
    installLayout->addWidget(installedLabel);
    installLayout->addStretch();
    installGroup = new QButtonGroup(this);
    installGroup->setExclusive(true);
    installButton = new QPushButton(tr("Install"), this);
    installButton->setCheckable(true);
    installGroup->addButton(installButton, 1);
    connect(installButton, SIGNAL(clicked()), this, SLOT(goInstallButton()));
    uninstallButton = new QPushButton(tr("Uninstall"), this);
    uninstallButton->setCheckable(true);
    installGroup->addButton(uninstallButton, 2);
    connect(uninstallButton, SIGNAL(clicked()), this, SLOT(goUninstallButton()));
    installLayout->addWidget(installButton);
    installLayout->addWidget(uninstallButton);
    mainLayout->addLayout(installLayout);
    if (isServiceInstalled())
    {
        installButton->setChecked(true);
        browseButton->setEnabled(false);
        pathEdit->setEnabled(false);
    }
    else
    {
        uninstallButton->setChecked(true);
        browseButton->setEnabled(true);
        pathEdit->setEnabled(true);
    }

    QHBoxLayout *startLayout = new QHBoxLayout();
    runningLabel = new QLabel(this);
    if (isServiceRunning())
        runningLabel->setText(tr("Service is running"));
    else
        runningLabel->setText(tr("Service is not running"));
    startLayout->addWidget(runningLabel);
    startLayout->addStretch();
    startGroup = new QButtonGroup(this);
    startGroup->setExclusive(true);
    startButton = new QPushButton(tr("Start"), this);
    startButton->setCheckable(true);
    isServiceInstalled() ? startButton->setEnabled(true) : startButton->setEnabled(false);
    startGroup->addButton(startButton, 1);
    connect(startButton, SIGNAL(clicked()), this, SLOT(goStartButton()));
    stopButton = new QPushButton(tr("Stop"), this);
    stopButton->setCheckable(true);
    isServiceInstalled() ? stopButton->setEnabled(true) : stopButton->setEnabled(false);
    startGroup->addButton(stopButton, 2);
    connect(stopButton, SIGNAL(clicked()), this, SLOT(goStopButton()));
    startLayout->addWidget(startButton);
    startLayout->addWidget(stopButton);
    mainLayout->addLayout(startLayout);
    if (isServiceRunning())
    {
        startButton->setChecked(true);
    }
    else
    {
        stopButton->setChecked(true);
    }

    this->setLayout(mainLayout);
}

QString Tab::getServicePath()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    return settings.value(SERVICE_PATH_STRING).toString();
}

bool Tab::isServiceInstalled()
{
    QtServiceController controller(SERVICE_NAME_STRING);
    return controller.isInstalled();
}

bool Tab::isServiceRunning()
{
    QtServiceController controller(SERVICE_NAME_STRING);
    return controller.isRunning();
}

void Tab::goInstallButton()
{
    if (QtServiceController::install(getServicePath()))
    {
        startButton->setEnabled(true);
        stopButton->setEnabled(true);
        installedLabel->setText(tr("Service is installed"));
        browseButton->setEnabled(false);
        pathEdit->setEnabled(false);
    }
}

void Tab::goUninstallButton()
{
    QtServiceController controller(SERVICE_NAME_STRING);
    if (controller.uninstall())
    {
        startButton->setEnabled(false);
        stopButton->setEnabled(false);
        installedLabel->setText(tr("Service is not installed"));
        browseButton->setEnabled(true);
        pathEdit->setEnabled(true);
    }
}

void Tab::goStartButton()
{
    QtServiceController controller(SERVICE_NAME_STRING);
    if (controller.start())
    {
        runningLabel->setText(tr("Service is running"));
        installButton->setEnabled(false);
        uninstallButton->setEnabled(false);
        emit driverStatus(true);
    }
}

void Tab::goStopButton()
{
    QtServiceController controller(SERVICE_NAME_STRING);
    if (controller.stop())
    {
        runningLabel->setText(tr("Service is not running"));
        installButton->setEnabled(true);
        uninstallButton->setEnabled(true);
        emit driverStatus(false);
    }
}

void Tab::goBrowse()
{
    QString tempPath;
    tempPath = QFileDialog::getOpenFileName(this, "Service Path", pathEdit->text());
    if (!tempPath.isEmpty())
    {
        pathEdit->setText(tempPath);
        QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
        settings.setValue(SERVICE_PATH_STRING, tempPath);
    }
}
