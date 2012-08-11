#ifndef SERVICETAB_H
#define SERVICETAB_H

#include <QWidget>

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
