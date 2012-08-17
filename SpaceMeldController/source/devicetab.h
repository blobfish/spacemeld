#ifndef DEVICETAB_H
#define DEVICETAB_H

#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include "deviceinfo.h"
#include "deviceconfig.h"

class QLabel;
class QStackedWidget;
class QProgressBar;
class QSplitter;

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
    void myReset(){this->reset();}

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
    void sendCommand(int command);
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
    int infoIndex;
    QStringList axisLabels;
};

class AxesView : public QTableView
{
    Q_OBJECT
public:
    explicit AxesView(QWidget *parent = 0);
public slots:
    void openEditors();
protected:
    int startDragIndex;
    QPoint startPosition;
    QPoint offset;
    QLabel *dragLabel;
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

class ButtonMapModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ButtonMapModel(QObject *parent, DeviceInfos &deviceInfosIn);
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
};

class ButtonMapView : public QTableView
{
    Q_OBJECT
public:
    explicit ButtonMapView(QWidget *parent = 0);
protected:
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
    void selectionUpdate(int index);
    void selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous);


private:
    void buildGui();

    TableView *view;
    TableModel *model;
    QTableView *axesView;
    AxesModel *axesModel;
    ButtonMapModel *buttonMapModel;
    ButtonMapView *buttonMapView;
    QStackedWidget *stack;
    DeviceInfos deviceInfos;
    QSplitter *subSplitter;
    QSplitter *mainSplitter;
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
signals:
    void currentIndexChanged(int index);
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
    QString displayText(const QVariant &value, const QLocale &locale) const;

private:
    double lower;
    double upper;
};

class KeyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    KeyDelegate(QWidget *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    QString displayText(const QVariant &value, const QLocale &locale) const;
};

class KeyDelegateEditLine : public QLineEdit
{
    Q_OBJECT
public:
    KeyDelegateEditLine(QWidget *parent = 0);
protected:
    virtual void keyPressEvent(QKeyEvent *event);
};
}

#endif // DEVICETAB_H
