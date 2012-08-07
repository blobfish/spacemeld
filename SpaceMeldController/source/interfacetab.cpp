#include <QtGui>
#include "definitions.h"
#include "interfacetab.h"

using namespace InterfaceGui;

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    buildGui();
    this->setToolTip((tr("Enable and Disable interfaces.\n"
                         "It is recommended to only enable the interfaces required for your 3D device\\s.\n"
                         "This should help minimize any conflicts with unknown devices.\n"
                         "Modifications here will require a driver restart to take affect.")));
}

void Tab::buildGui()
{
    QHBoxLayout *layout = new QHBoxLayout();

    QVBoxLayout *statusLayout = new QVBoxLayout();
    statusLayout->addStretch();
    interfaceButtonGroup = new QButtonGroup(this);
    interfaceEnableButton = new QPushButton("Enable", this);
    interfaceEnableButton->setCheckable(true);
    interfaceEnableButton->setEnabled(false);
    interfaceDisableButton = new QPushButton("Disable", this);
    interfaceDisableButton->setCheckable(true);
    interfaceDisableButton->setEnabled(false);
    interfaceButtonGroup->addButton(interfaceEnableButton);
    interfaceButtonGroup->addButton(interfaceDisableButton);
    interfaceButtonGroup->setExclusive(true);
    statusLayout->addWidget(interfaceEnableButton);
    statusLayout->addWidget(interfaceDisableButton);

    interfaceListWidget = new QListWidget(this);
    interfaceListWidget->addItem(INTERFACE_STRING_SERIAL);
    interfaceListWidget->addItem(INTERFACE_STRING_USB);
    interfaceListWidget->addItem(INTERFACE_STRING_JOYSTICK);

    layout->addWidget(interfaceListWidget);
    layout->addLayout(statusLayout);

    this->setLayout(layout);

    connect(interfaceListWidget, SIGNAL(currentTextChanged(QString)), this, SLOT(goInterfaceItemChanged(QString)));
    connect(interfaceButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(goInterfaceButton(int)));
}

void Tab::goInterfaceItemChanged(const QString & currentText)
{
    if (!interfaceEnableButton->isEnabled())
        interfaceEnableButton->setEnabled(true);

    if (!interfaceDisableButton->isEnabled())
        interfaceDisableButton->setEnabled(true);

    interfaceSelection = currentText;

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
