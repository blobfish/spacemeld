#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QIcon>
#include "userpassworddialog.h"

UserPasswordDialog::UserPasswordDialog(QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
    buildGui();
}

void UserPasswordDialog::buildGui()
{
    this->setWindowIcon(QIcon(":/icon/spacemeld_thumbnail.png"));
    this->setWindowTitle(tr("Service Credentials"));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QLabel *usernameLabel = new QLabel("Username:", this);
    QLabel *passwordLabel = new QLabel("Password:", this);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->addWidget(usernameLabel);
    usernameLayout->addWidget(usernameEdit);
    mainLayout->addLayout(usernameLayout);
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);
    mainLayout->addLayout(passwordLayout);

    QDialogButtonBox *buttons = new QDialogButtonBox
            (QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    mainLayout->addWidget(buttons);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    this->setLayout(mainLayout);
}

bool UserPasswordDialog::validate() const
{
    if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty())
        return false;
    return true;
}

QString UserPasswordDialog::getUsername() const
{
    return usernameEdit->text();
}

QString UserPasswordDialog::getPassword() const
{
    return passwordEdit->text();
}
