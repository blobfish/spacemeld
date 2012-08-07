#ifndef INTERFACETAB_H
#define INTERFACETAB_H

#include <QWidget>

class QPushButton;
class QButtonGroup;
class QListWidget;

namespace InterfaceGui
{

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = 0);

private slots:
    void goInterfaceButton(int id);
    void goInterfaceItemChanged(const QString &currentText);
    
private:
    void buildGui();
    QPushButton *interfaceEnableButton;
    QPushButton *interfaceDisableButton;
    QButtonGroup *interfaceButtonGroup;
    QListWidget *interfaceListWidget;
    QString interfaceSelection;
    
};
}

#endif // INTERFACETAB_H
