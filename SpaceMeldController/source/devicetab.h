#ifndef DEVICETAB_H
#define DEVICETAB_H

#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include "deviceinfo.h"
#include "deviceconfig.h"

class QLabel;
class QStackedWidget;
class QProgressBar;

namespace DeviceGui
{
class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void readData();

private:
    QStringList headerStrings;
    DeviceInfos deviceInfos;
};

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = 0);

public slots:
    void driverStatus(bool signal);

private slots:
    void driverStatusHelper();

private:
    void buildGui();

    TableView *view;
    TableModel *model;
    QStackedWidget *stack;
};

class BoolDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    BoolDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

class OutputDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    OutputDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};
}

#endif // DEVICETAB_H
