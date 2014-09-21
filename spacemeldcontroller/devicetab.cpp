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

#include <QtGui/QtGui>
#include <QComboBox>
#include <QCheckBox>
#include <QKeySequence>
#include "qtservice.h"
#include "definitions.h"
#include "devicetab.h"

using namespace DeviceGui;

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    buildGui();
    QtServiceController controller(SERVICE_NAME_STRING);
    if (controller.isRunning())
    {
        //we need to make sure that the service is through the init.
        this->driverStatusHelper();
    }
    else
        stack->setCurrentIndex(0);
}

void Tab::buildGui()
{
    stack = new QStackedWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *label = new QLabel(stack);
    label->setText("Service is not running");
    label->setAlignment(Qt::AlignHCenter);
    label->setAlignment(Qt::AlignCenter);
    stack->addWidget(label);

    QWidget *progressWidget = new QWidget(stack);
    QVBoxLayout *progressLayout = new QVBoxLayout();
    progressWidget->setLayout(progressLayout);
    QLabel *progressLabel = new QLabel(progressWidget);
    progressLabel->setText("Service initializing");
    progressLabel->setAlignment(Qt::AlignHCenter);
    progressLayout->addWidget(progressLabel);
    QProgressBar *progress = new QProgressBar(progressWidget);
    progressLayout->addWidget(progress);
    progressLayout->addStretch();
    progress->setMinimum(0);
    progress->setMaximum(0);
    stack->addWidget(progressWidget);

    mainSplitter = new QSplitter(stack);
    mainSplitter->setOrientation(Qt::Vertical);

    view = new TableView(mainSplitter);
    view->setToolTip(tr("Device list:\n"
                        "   enabled to false causes service to ignore the device.\n"
                        "   Changes to enabled will require a service restart."));
    model = new TableModel(mainSplitter, deviceInfos);
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    BoolDelegate *enabledDelegate = new BoolDelegate(view);
    view->setItemDelegateForColumn(1, enabledDelegate);
    OutputDelegate *outputDelegate = new OutputDelegate(view);
    connect(outputDelegate, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionUpdate(int)));
    view->setItemDelegateForColumn(5, outputDelegate);
    mainSplitter->addWidget(view);

    subSplitter = new QSplitter(mainSplitter);
    subSplitter->setOrientation(Qt::Horizontal);

    exportModel = new ExportModel(subSplitter, deviceInfos);
    exportView = new QTableView(subSplitter);
    exportView->setToolTip(tr("Export Communication:\n"
                              "   Devices can be mapping through different export protocols.\n"
                              "      X11: for unix/linux proprietary, legacy compatibility.\n"
                              "      Win: for windows proprietary, legacy compatibility.\n"
                              "      Mac: for macosx proprietary, legacy compatibility.\n"
                              "      DBUS: for dbus compatibility. (new development).\n"
                              "         axes and button configuration for DBUS are done at client.\n"
                              "   Changes here require a service restart to take affect."));
    exportView->setModel(exportModel);
    exportView->setSelectionMode(QAbstractItemView::SingleSelection);
    BoolDelegate *exportDelegate = new BoolDelegate(exportView);
    exportView->setItemDelegateForColumn(1, exportDelegate);
    connect(view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), exportModel,
            SLOT(selectionChangedSlot(QModelIndex,QModelIndex)));

    subSplitter->addWidget(exportView);

    axesView = new AxesView(subSplitter);
    axesView->setToolTip(tr("Axes Parameters:\n"
                            "   Check inverse to reverse axis direction.\n"
                            "   Set scale number for sensitivity. .1 to 10.0\n"
                            "   Drag and drop output axes to change device axes mapping.\n"
                            "   Changes here should take affect immediately and not require a service restart.\n"
                            "      Assuming the relevant device was detected and enabled upon service start."));
    axesModel = new AxesModel(subSplitter, deviceInfos);
    axesView->setModel(axesModel);
    InverseDelegate *inverseDelegate = new InverseDelegate(axesView);
    axesView->setItemDelegateForColumn(1, inverseDelegate);
    subSplitter->addWidget(axesView);
    connect(view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), axesModel,
            SLOT(selectionChangedSlot(QModelIndex,QModelIndex)));
    connect(exportView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), axesModel,
            SLOT(exportChangedSlot(QModelIndex,QModelIndex)));
    connect(axesModel, SIGNAL(modelReset()), axesView, SLOT(openEditors()));

    ScaleDelegate *scaleDelegate = new ScaleDelegate(axesView);
    axesView->setItemDelegateForColumn(2, scaleDelegate);
    axesView->setSelectionMode(QAbstractItemView::SingleSelection);

    buttonMapModel = new ButtonMapModel(subSplitter, deviceInfos);
    buttonMapView = new ButtonMapView(subSplitter);
    buttonMapView->setToolTip(tr("Button Mapping:\n"
                                 "   Edit(double click) accelerator table cell and press accelerator keys.\n"
                                 "   Changes here should take affect immediately and not require a service restart.\n"
                                 "      Assuming the relevant device was detected and enabled upon service start."));
    buttonMapView->setModel(buttonMapModel);
    KeyDelegate *keyDelegate = new KeyDelegate(buttonMapView);
    buttonMapView->setItemDelegateForColumn(1, keyDelegate);
    connect(view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), buttonMapModel,
            SLOT(selectionChangedSlot(QModelIndex,QModelIndex)));
    connect(exportView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), buttonMapModel,
            SLOT(exportChangedSlot(QModelIndex,QModelIndex)));
    subSplitter->addWidget(buttonMapView);

    mainSplitter->addWidget(subSplitter);
    stack->addWidget(mainSplitter);

    mainLayout->addWidget(stack);
    this->setLayout(mainLayout);

    connect(mainSplitter, SIGNAL(splitterMoved(int,int)), this, SLOT(saveSplittersSlot(int,int)));
    connect(subSplitter, SIGNAL(splitterMoved(int,int)), this, SLOT(saveSplittersSlot(int,int)));
    loadSplitters();
}

void Tab::driverStatus(bool signal)
{
    //the controller starts the service and immediately returns.
    //service initialization can take a while especially for the serials.
    //Service writes to config file after initialization is done.
    //So we wait for that write before we show view and populate model.

    if (signal)
    {
        QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
        bool status = settings.value(SERVICE_STATUS_STRING).toBool();
        if (!status)
        {
            QTimer::singleShot(1000, this, SLOT(driverStatusHelper()));
            if (stack->currentIndex() != 1)
                stack->setCurrentIndex(1);
        }
        else
        {
            deviceInfos = DeviceConfig::readConfiguredDevices();
            view->selectionModel()->clear();
            model->myReset();
            stack->setCurrentIndex(2);
        }
    }
    else
        stack->setCurrentIndex(0);
}

void Tab::driverStatusHelper()
{
    this->driverStatus(true);
}

void Tab::selectionUpdate(int index)
{
//    OutputType::Output output = static_cast<OutputType::Output>(index);
//    if (output == OutputType::DBUS || output == OutputType::UNKNOWN)
//        subSplitter->hide();
//    else
//        subSplitter->show();
}

void Tab::saveSplittersSlot(int pos, int index)
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    settings.setValue("mainSplitter", mainSplitter->saveState());
    settings.setValue("subSplitter", subSplitter->saveState());
}

void Tab::loadSplitters()
{
    QSettings settings(QSettings::SystemScope, ORG_NAME_STRING, APP_NAME_STRING);
    if (settings.contains("mainSplitter"))
        mainSplitter->restoreState(settings.value("mainSplitter").toByteArray());
    if (settings.contains("subSplitter"))
        subSplitter->restoreState(settings.value("subSplitter").toByteArray());
}

TableModel::TableModel(QObject *parent, DeviceInfos &deviceInfosIn) : QAbstractTableModel(parent),
    deviceInfos(deviceInfosIn)
{
    headerStrings.push_back("Name");
    headerStrings.push_back("Enabled");
    headerStrings.push_back("Detected");
    headerStrings.push_back("Interface");
    headerStrings.push_back("Path");
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return deviceInfos.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return headerStrings.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int column = index.column();
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return deviceInfos.at(index.row()).modelName;
        case 1:
            return deviceInfos.at(index.row()).enabled;
        case 2:
            return deviceInfos.at(index.row()).detected;
        case 3:
            return ConnectionInterfaceType::getString(deviceInfos.at(index.row()).interfaceId);
        case 4:
            return deviceInfos.at(index.row()).path;
        }
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    return headerStrings.at(section);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
    case 0:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    case 1:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    case 2:
        return Qt::NoItemFlags;
    case 3:
        return Qt::NoItemFlags;
    case 4:
        return Qt::NoItemFlags;
    case 5:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index);
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    switch (index.column())
    {
    case 1:
        deviceInfos[index.row()].enabled = value.toBool();
        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    emit dataChanged(index, index);
    return true;
}

QWidget* BoolDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *box = new QComboBox(parent);
    box->setEditable(false);
    return box;
}

void BoolDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *box = qobject_cast<QComboBox*>(editor);
    if (!box)
        return;
    box->addItem("True");
    box->addItem("False");
    if (index.data().toBool())
        box->setCurrentIndex(0);
    else
        box->setCurrentIndex(1);
}

void BoolDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = qobject_cast<QComboBox*>(editor);
    if (!box)
        return;
    if (box->currentIndex() == 0)
        model->setData(index, true);
    else
        model->setData(index, false);
}

QWidget* OutputDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *box = new QComboBox(parent);
    box->setEditable(false);
    connect(box, SIGNAL(currentIndexChanged(int)), this, SIGNAL(currentIndexChanged(int)));
    return box;
}

void OutputDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QComboBox *box = qobject_cast<QComboBox*>(editor);
    if (!box)
        return;
    for (int position(0); position < OutputType::size(); ++position)
        box->addItem(OutputType::getString(static_cast<OutputType::Output>(position)));
    box->setCurrentIndex(index.data(Qt::EditRole).toInt());
}

void OutputDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QComboBox *box = qobject_cast<QComboBox*>(editor);
    if (!box)
        return;
    model->setData(index, box->currentIndex());
}

AxesModel::AxesModel(QObject *parent, DeviceInfos &deviceInfosIn) : QAbstractTableModel(parent),
    deviceInfos(deviceInfosIn), infoIndex(-1), exportId(-1)
{
    headerStrings.push_back(tr("Device Axis"));
    headerStrings.push_back(tr("Inverse"));
    headerStrings.push_back(tr("Scale"));
    headerStrings.push_back(tr("Output Axis"));

    axisLabels.push_back(tr("Translation X"));
    axisLabels.push_back(tr("Translation Y"));
    axisLabels.push_back(tr("Translation Z"));
    axisLabels.push_back(tr("Rotation X"));
    axisLabels.push_back(tr("Rotation Y"));
    axisLabels.push_back(tr("Rotation Z"));
}

int AxesModel::rowCount(const QModelIndex &parent) const
{
    if (infoIndex == -1 || exportId == -1)
        return 0;
    return 6;
}

int AxesModel::columnCount(const QModelIndex &parent) const
{
    if (infoIndex == -1 || exportId == -1)
        return 0;
    return headerStrings.size();
}

QVariant AxesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    DeviceInfo temp = deviceInfos.at(infoIndex);
    int column = index.column();
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return axisLabels.at(index.row());
        case 1:
            return temp.exports.at(exportId).inverse.at(index.row());
        case 2:
            return temp.exports.at(exportId).scale.at(index.row());
        case 3:
            return axisLabels.at(temp.exports.at(exportId).axesMap.at(index.row()));
        }
    }
    if (role == Qt::EditRole)
    {
        switch (column)
        {
        case 3:
            return deviceInfos.at(infoIndex).exports.at(exportId).axesMap.at(index.row());
            return 0;
        }
    }
    return QVariant();
}

QVariant AxesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    return headerStrings.at(section);
}

Qt::ItemFlags AxesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
    case 0:
        return Qt::NoItemFlags;//device axis.
    case 1:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;//inverse
    case 2:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;//scale
    case 3:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled |//output axis
                Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }

    return QAbstractTableModel::flags(index);
}

bool AxesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    switch (index.column())
    {
    case 1:
        deviceInfos[infoIndex].exports[exportId].inverse[index.row()] = value.toInt();
        break;
    case 2:
        deviceInfos[infoIndex].exports[exportId].scale[index.row()] = value.toDouble();
        break;
    case 3:
        deviceInfos[infoIndex].exports[exportId].axesMap[index.row()] = value.toInt();
        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    //the dragging of column 3 (axis map) makes 2 calls to this set data function.
    //the following complication is to send the command for only one call.
    //ignore the first, send on the second.
    if (index.column() == 3)
    {
        static bool signal = false; //false represents first call (ignore).
        if (signal)
        {
            sendCommand(100 + deviceInfos[infoIndex].runTimeId);
            signal = false;
        }
        else
            signal = true;
    }
    else
        sendCommand(100 + deviceInfos[infoIndex].runTimeId);


    emit dataChanged(index, index);
    return true;
}

void AxesModel::sendCommand(int command)
{
    QtServiceController controller(SERVICE_NAME_STRING);
    if (!controller.sendCommand(command))
        qDebug() << "send command failed";
}

void AxesModel::selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    this->beginResetModel();
    if (current.isValid())
        infoIndex = current.row();
    else
        infoIndex = -1;
    exportId = -1;//user has to reselect the output
    this->endResetModel();
}

void AxesModel::exportChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    this->beginResetModel();
    if (current.isValid())
    {
        if (OutputType::getType(current.row()) == OutputType::DBUS)
            exportId = -1;
        else
            exportId = current.row();
    }
    else
        exportId = -1;
    this->endResetModel();
}

AxesView::AxesView(QWidget *parent) : QTableView(parent), startDragIndex(-1)
{
    dragLabel = new QLabel(this);
    dragLabel->setFrameStyle(QFrame::Panel);
    QPalette palette = dragLabel->palette();
    palette.setColor(dragLabel->backgroundRole(), Qt::yellow);
    dragLabel->setPalette(palette);
    dragLabel->setAutoFillBackground(true);
    dragLabel->hide();
}

void AxesView::openEditors()
{
    this->openPersistentEditor(this->model()->index(0, 1));
    this->openPersistentEditor(this->model()->index(1, 1));
    this->openPersistentEditor(this->model()->index(2, 1));
    this->openPersistentEditor(this->model()->index(3, 1));
    this->openPersistentEditor(this->model()->index(4, 1));
    this->openPersistentEditor(this->model()->index(5, 1));
}

void AxesView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex tempIndex = this->indexAt(event->pos());
    if (tempIndex.isValid())
    {
        if (tempIndex.column() == 3)
        {
            dragLabel->resize(this->columnWidth(tempIndex.column()), this->rowHeight(tempIndex.row()));
            int cornerX(0);
            for (int index = 0; index < tempIndex.column(); ++index)
                cornerX += this->columnWidth(index);
            int cornerY(0);
            for (int index = 0; index < tempIndex.row(); ++index)
                cornerY += this->rowHeight(index);
            startPosition = event->pos();
            offset.rx() = startPosition.x() - cornerX;
            offset.ry() = cornerY - startPosition.y();
            startDragIndex = tempIndex.row();
        }
    }
    QTableView::mousePressEvent(event);
}

void AxesView::mouseMoveEvent(QMouseEvent *event)
{
    if (startDragIndex != -1)
    {
        QPoint temp = event->pos() - startPosition;
        if (temp.manhattanLength() > 3 && dragLabel->isHidden())
        {
            QModelIndex tempIndex = this->indexAt(event->pos());
            dragLabel->setText(this->model()->data(tempIndex).toString());
            dragLabel->setVisible(true);
        }
        dragLabel->move(event->pos() - offset);
    }
    QTableView::mouseMoveEvent(event);
}

void AxesView::mouseReleaseEvent(QMouseEvent *event)
{
    if (dragLabel->isVisible())
        dragLabel->hide();
    if (startDragIndex != -1)
    {
        QModelIndex tempIndex = this->indexAt(event->pos());
        if (tempIndex.isValid())
        {
            if (tempIndex.column() == 3 && tempIndex.row() != startDragIndex)
            {
                //swap values
                int first = this->model()->data(this->model()->index(startDragIndex, 3), Qt::EditRole).toInt();
                int second = this->model()->data(this->model()->index(tempIndex.row(), 3), Qt::EditRole).toInt();
                this->model()->setData(this->model()->index(startDragIndex, 3), second);
                this->model()->setData(this->model()->index(tempIndex.row(), 3), first);
            }
        }
        startDragIndex = -1;
    }
    QTableView::mouseReleaseEvent(event);
}

QWidget* InverseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *box = new QCheckBox(parent);
    return box;
}

void InverseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QCheckBox *box = qobject_cast<QCheckBox*>(editor);
    if (!box)
        return;
    if (index.data() == -1)
        box->setCheckState(Qt::Checked);
    else
        box->setCheckState(Qt::Unchecked);
}

void InverseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QCheckBox *box = qobject_cast<QCheckBox*>(editor);
    if (!box)
        return;
    if (box->isChecked())
        model->setData(index, -1);
    else
        model->setData(index, 1);
}

ScaleDelegate::ScaleDelegate(QWidget *parent) : QStyledItemDelegate(parent), lower(0.1), upper(10.0)
{

}

QWidget* ScaleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *edit = new QLineEdit(parent);
    QDoubleValidator *validate = new QDoubleValidator(edit);
    validate->setBottom(lower);
    validate->setTop(upper);
    validate->setDecimals(1);
    edit->setValidator(validate);
    return edit;
}

void ScaleDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
    if (!edit)
        return;
    edit->setText(index.data().toString());
}

void ScaleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    QLineEdit *edit = qobject_cast<QLineEdit*>(editor);
    if (!edit)
        return;
    double temp = edit->text().toDouble();
    if (temp < lower || temp > upper)
        return;
    model->setData(index, edit->text().toDouble());
}

QString ScaleDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    return locale.toString(value.toFloat(), 'f', 1);
}

ButtonMapModel::ButtonMapModel(QObject *parent, DeviceInfos &deviceInfosIn) : QAbstractTableModel(parent),
    deviceInfos(deviceInfosIn), infoIndex(-1), exportId(-1)
{
    headerStrings.push_back(tr("Button Number"));
    headerStrings.push_back(tr("Accelerator"));
}

int ButtonMapModel::rowCount(const QModelIndex &parent) const
{
    if (infoIndex == -1 || exportId == -1)
        return 0;
    return deviceInfos.at(infoIndex).buttonCount;
}

int ButtonMapModel::columnCount(const QModelIndex &parent) const
{
    if (infoIndex == -1 || exportId == -1)
        return 0;
    return 2;
}

QVariant ButtonMapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int column = index.column();
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return QLocale::system().toString(index.row() + 1);
        case 1:
            return deviceInfos.at(infoIndex).exports.at(exportId).buttonKeyMap.value(index.row());
        }
    }
    if (role == Qt::EditRole)
    {
        switch (column)
        {
        case 3:
            return deviceInfos.at(infoIndex).exports.at(exportId).axesMap.at(index.row());
        }
    }
    return QVariant();
}

QVariant ButtonMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    return headerStrings.at(section);
}

Qt::ItemFlags ButtonMapModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
    case 0:
        return Qt::NoItemFlags;//device axis.
    case 1:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;//inverse
    }

    return QAbstractTableModel::flags(index);
}

bool ButtonMapModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    switch (index.column())
    {
    case 1:
        deviceInfos[infoIndex].exports[exportId].buttonKeyMap.insert(index.row(), value.toString());
        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    QtServiceController controller(SERVICE_NAME_STRING);
    if (!controller.sendCommand(200 + deviceInfos[infoIndex].runTimeId))
        qDebug() << "send command failed";

    emit dataChanged(index, index);
    return true;
}

void ButtonMapModel::selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    this->beginResetModel();
    if (current.isValid())
        infoIndex = current.row();
    else
        infoIndex = -1;
    exportId = -1;//user has to reselect the output
    this->endResetModel();
}

void ButtonMapModel::exportChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    this->beginResetModel();
    if (current.isValid())
    {
        if (OutputType::getType(current.row()) == OutputType::DBUS)
            exportId = -1;
        else
            exportId = current.row();
    }
    else
        exportId = -1;
    this->endResetModel();
}

ExportModel::ExportModel(QObject *parent, DeviceInfos &deviceInfosIn) : QAbstractTableModel(parent),
    deviceInfos(deviceInfosIn), infoIndex(-1)
{
    headerStrings.push_back(tr("Export"));
    headerStrings.push_back(tr("Enabled"));
}

int ExportModel::rowCount(const QModelIndex &parent) const
{
    if (infoIndex == -1)
        return 0;
    return OutputType::size();
}

int ExportModel::columnCount(const QModelIndex &parent) const
{
    if (infoIndex == -1)
        return 0;
    return headerStrings.size();
}

QVariant ExportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    int column = index.column();
    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return OutputType::getString(index.row());
        case 1:
            return deviceInfos.at(infoIndex).exports.at(index.row()).enabled;
        }
    }
    return QVariant();
}

QVariant ExportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    return headerStrings.at(section);
}

Qt::ItemFlags ExportModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    switch (index.column())
    {
    case 0:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    case 1:
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;//inverse
    }

    return QAbstractTableModel::flags(index);
}

bool ExportModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    switch (index.column())
    {
    case 1:
        deviceInfos[infoIndex].exports[index.row()].enabled = value.toBool();
        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    QtServiceController controller(SERVICE_NAME_STRING);
    if (!controller.sendCommand(200 + deviceInfos[infoIndex].runTimeId))
        qDebug() << "send command failed";

    emit dataChanged(index, index);
    return true;
}

void ExportModel::selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    this->beginResetModel();
    if (current.isValid())
        infoIndex = current.row();
    else
        infoIndex = -1;
    this->endResetModel();
}

ButtonMapView::ButtonMapView(QWidget *parent) : QTableView(parent)
{

}

KeyDelegate::KeyDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

QWidget* KeyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    KeyDelegateEditLine *edit = new KeyDelegateEditLine(parent);
    return edit;
}

void KeyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void KeyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (!index.isValid())
        return;
    KeyDelegateEditLine *edit = qobject_cast<KeyDelegateEditLine*>(editor);
    if (!edit)
        return;
    QKeySequence sequence(edit->text(), QKeySequence::NativeText);
    model->setData(index, sequence.toString(QKeySequence::PortableText));
}

QString KeyDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    QKeySequence sequence(value.toString(), QKeySequence::PortableText);
    return sequence.toString(QKeySequence::NativeText);
}

KeyDelegateEditLine::KeyDelegateEditLine(QWidget *parent) : QLineEdit(parent)
{

}

void KeyDelegateEditLine::keyPressEvent(QKeyEvent *event)
{
    QKeySequence sequence(event->modifiers() + event->key(), QKeySequence::NativeText);
    this->setText(sequence.toString(QKeySequence::NativeText));
    event->accept();
}
