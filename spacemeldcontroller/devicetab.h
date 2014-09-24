/*
SpaceMeld is a driver/API for spaceballs, spacemice, spacenavigators etc..
Copyright (C) 2014 Thomas Anderson blobfish[at]gmx.com

This file is part of SpaceMeld.

SpaceMeld is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SpaceMeld is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SpaceMeld.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEVICETAB_H
#define DEVICETAB_H

#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QSortFilterProxyModel>
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
    explicit TableView(QWidget *parent = 0) : QTableView(parent){}
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
    void exportChangedSlot(const QModelIndex &current, const QModelIndex &previous);

private:
    void sendCommand(int command);
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
    int infoIndex;
    int exportId;
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
    void exportChangedSlot(const QModelIndex &current, const QModelIndex &previous);

private:
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
    int infoIndex;
    int exportId;
};

class ExportModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ExportModel(QObject *parent, DeviceInfos &deviceInfosIn);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool showAxisButtonMap(const QModelIndex &index);
public slots:
    void selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous);

private:
    QStringList headerStrings;
    DeviceInfos &deviceInfos;
    int infoIndex;
};

class ExportFilterModel : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit ExportFilterModel(QObject *parent);
protected:
  virtual bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const override;
private:
  std::vector<bool> exportPredicates;
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
    void exportSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private slots:
    void driverStatusHelper();
    void selectionUpdate(int index);
    void saveSplittersSlot(int pos, int index);

private:
    void buildGui();
    void loadSplitters();

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
    QTableView *exportView;
    ExportModel *exportModel;
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
