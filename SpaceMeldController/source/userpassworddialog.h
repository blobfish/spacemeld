#ifndef USERPASSWORDDIALOG_H
#define USERPASSWORDDIALOG_H

#include <QDialog>
class QLineEdit;

class UserPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserPasswordDialog(QWidget *parent = 0);
    bool validate() const;
    QString getUsername() const;
    QString getPassword() const;
private:
    void buildGui();

    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};

#endif // USERPASSWORDDIALOG_H
