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
    explicit TableModel(QObject *parent, DeviceInfos &deviceInfosIn);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
};

class AxesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AxesModel(QObject *parent, DeviceInfos &deviceInfosIn);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
public slots:
    void selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous);

private:
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
    int infoIndex;
    QStringList axisLabels;
};

class AxesView : public QTableView
{
    Q_OBJECT
public:
    explicit AxesView(QWidget *parent = 0) : QTableView(parent), startDragIndex(-1){}
public slots:
    void openEditors();
protected:
    int startDragIndex;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
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
    QTableView *axesView;
    AxesModel *axesModel;
    QStackedWidget *stack;
    DeviceInfos deviceInfos;
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

class InverseDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    InverseDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

class ScaleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ScaleDelegate(QWidget *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    double lower;
    double upper;
};
}

#endif // DEVICETAB_H
