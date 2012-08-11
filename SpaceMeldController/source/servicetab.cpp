#include <QSettings>
#include <QtGui/QtGui>

#include "qtservice.h"

#include "definitions.h"
#include "servicetab.h"

using namespace ServiceGui;

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    initialize();
}

void Tab::initialize()
{
    //service path.
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QString pathMessage(tr("This is the path to the service executable."
                           "\nService must be stopped and uninstalled to modify."));

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

    //interface settings
    QGroupBox *groupBox = new QGroupBox(tr("Device Interfaces"), this);
    QHBoxLayout *interfaceLayout = new QHBoxLayout();

    QVBoxLayout *statusLayout = new QVBoxLayout();
    interfaceButtonGroup = new QButtonGroup(this);
    interfaceEnableButton = new QPushButton(tr("Enable"), this);
    interfaceEnableButton->setToolTip(tr("Press to enable the selected interface.\nService must be stopped."));
    interfaceEnableButton->setCheckable(true);
    interfaceEnableButton->setEnabled(false);
    interfaceDisableButton = new QPushButton(tr("Disable"), this);
    interfaceDisableButton->setToolTip(tr("Press to disable the selected interface.\nService must be stopped."));
    interfaceDisableButton->setCheckable(true);
    interfaceDisableButton->setEnabled(false);
    interfaceButtonGroup->addButton(interfaceEnableButton);
    interfaceButtonGroup->addButton(interfaceDisableButton);
    interfaceButtonGroup->setExclusive(true);
    statusLayout->addWidget(interfaceEnableButton);
    statusLayout->addWidget(interfaceDisableButton);
    statusLayout->addStretch();

    interfaceListWidget = new QListWidget(this);
    interfaceListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    interfaceListWidget->addItem(tr(INTERFACE_STRING_SERIAL));
    interfaceListWidget->addItem(tr(INTERFACE_STRING_USB));
    interfaceListWidget->addItem(tr(INTERFACE_STRING_JOYSTICK));
    interfaceListWidget->setToolTip((tr("Enable and Disable interfaces. Service must be stopped to modify\n"
                                        "It is recommended to only enable the interfaces required for your 3D device\\s.\n"
                                        "This should help minimize any conflicts with unknown devices.\n"
                                        "Modifications here will require a driver restart to take affect.")));

    interfaceLayout->addWidget(interfaceListWidget);
    interfaceLayout->addLayout(statusLayout);
    groupBox->setLayout(interfaceLayout);

    mainLayout->addWidget(groupBox);

    connect(interfaceListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(goInterfaceItemChanged()));
    connect(interfaceButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(goInterfaceButton(int)));

    //install start group.
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
    installButton->setToolTip(tr("Press to install the service."));
    installButton->setCheckable(true);
    installGroup->addButton(installButton, 1);
    connect(installButton, SIGNAL(clicked()), this, SLOT(goInstallButton()));
    uninstallButton = new QPushButton(tr("Uninstall"), this);
    uninstallButton->setToolTip(tr("Press to uninstall the service."));
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
    startButton->setToolTip(tr("Press to start the service."));
    startButton->setCheckable(true);
    isServiceInstalled() ? startButton->setEnabled(true) : startButton->setEnabled(false);
    startGroup->addButton(startButton, 1);
    connect(startButton, SIGNAL(clicked()), this, SLOT(goStartButton()));
    stopButton = new QPushButton(tr("Stop"), this);
    stopButton->setToolTip(tr("Press to stop the service."));
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
        interfaceListWidget->setEnabled(false);
    }
    else
    {
        stopButton->setChecked(true);
        interfaceListWidget->setEnabled(true);
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
        interfaceListWidget->setEnabled(false);
        interfaceEnableButton->setEnabled(false);
        interfaceDisableButton->setEnabled(false);
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
        interfaceListWidget->setEnabled(true);
        if (interfaceListWidget->selectionModel()->hasSelection())
        {
            interfaceEnableButton->setEnabled(true);
            interfaceDisableButton->setEnabled(true);
        }
        emit driverStatus(false);
    }
}

void Tab::goBrowse()
{
    QString tempPath;
    tempPath = QFileDialog::getOpenFileName(this, tr("Service Path"), pathEdit->text());
    if (!tempPath.isEmpty())
    {
        pathEdit->setText(tempPath);
        QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
        settings.setValue(SERVICE_PATH_STRING, tempPath);
    }
}

void Tab::goInterfaceItemChanged()
{
    if (!interfaceEnableButton->isEnabled())
        interfaceEnableButton->setEnabled(true);

    if (!interfaceDisableButton->isEnabled())
        interfaceDisableButton->setEnabled(true);

    interfaceSelection = interfaceListWidget->currentItem()->text();

    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup(INTERFACE_STRING);
    bool enabled = settings.value(interfaceSelection, false).toBool();

    if (enabled)
        interfaceEnableButton->setChecked(true);
    else
        interfaceDisableButton->setChecked(true);
}

void Tab::goInterfaceButton(int id)
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.beginGroup(INTERFACE_STRING);
    if (id == interfaceButtonGroup->id(interfaceEnableButton))
        settings.setValue(interfaceSelection, true);
    else
        settings.setValue(interfaceSelection, false);
}
