#include <QtGui/QtGui>
#include <QComboBox>
#include <QCheckBox>
#include "qtservice.h"
#include "definitions.h"
#include "devicetab.h"

using namespace DeviceGui;

Tab::Tab(QWidget *parent) :
    QWidget(parent)
{
    deviceInfos = DeviceConfig::readConfiguredDevices();
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
    this->setToolTip("The device tab sets options for devices.\n Modifications here "
                        "will require a driver restart to take effect.");
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

    QSplitter *deviceContainer = new QSplitter(stack);
    deviceContainer->setOrientation(Qt::Vertical);
    QVBoxLayout *deviceLayout = new QVBoxLayout();
    deviceContainer->setLayout(deviceLayout);

    view = new TableView(deviceContainer);
    model = new TableModel(deviceContainer, deviceInfos);
    view->setModel(model);
    BoolDelegate *enabledDelegate = new BoolDelegate(view);
    view->setItemDelegateForColumn(1, enabledDelegate);
    OutputDelegate *outputDelegate = new OutputDelegate(view);
    view->setItemDelegateForColumn(5, outputDelegate);
    deviceLayout->addWidget(view);

    axesView = new AxesView;
    axesModel = new AxesModel(deviceContainer, deviceInfos);
    axesView->setModel(axesModel);
    InverseDelegate *inverseDelegate = new InverseDelegate(axesView);
    axesView->setItemDelegateForColumn(1, inverseDelegate);
    deviceLayout->addWidget(axesView);
    connect(view->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), axesModel,
            SLOT(selectionChangedSlot(QModelIndex,QModelIndex)));
    connect(axesModel, SIGNAL(modelReset()), axesView, SLOT(openEditors()));

    stack->addWidget(deviceContainer);

    mainLayout->addWidget(stack);
    this->setLayout(mainLayout);
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

TableModel::TableModel(QObject *parent, DeviceInfos &deviceInfosIn) : QAbstractTableModel(parent),
    deviceInfos(deviceInfosIn)
{
    headerStrings.push_back("Name");
    headerStrings.push_back("Enabled");
    headerStrings.push_back("Detected");
    headerStrings.push_back("Interface");
    headerStrings.push_back("Path");
    headerStrings.push_back("Output");
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return deviceInfos.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return 6;
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
        case 5:
            return OutputType::getString(deviceInfos.at(index.row()).output);
        }
    }
    if (role == Qt::EditRole)
    {
        switch (column)
        {
        case 5: return static_cast<int>(deviceInfos.at(index.row()).output);
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
    case 5:
        deviceInfos[index.row()].output = static_cast<OutputType::Output>(value.toInt());
        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    emit dataChanged(index, index);
    return true;
}

TableView::TableView(QWidget *parent) : QTableView(parent)
{

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
    deviceInfos(deviceInfosIn), infoIndex(-1)
{
    headerStrings.push_back("Device Axis");
    headerStrings.push_back("Inverse");
    headerStrings.push_back("Scale");
    headerStrings.push_back("Output Axis");

    axisLabels.push_back("Translation X");
    axisLabels.push_back("Translation Y");
    axisLabels.push_back("Translation Z");
    axisLabels.push_back("Rotation X");
    axisLabels.push_back("Rotation Y");
    axisLabels.push_back("Rotation Z");
}

int AxesModel::rowCount(const QModelIndex &parent) const
{
    if (infoIndex == -1)
        return 0;
    return 6;
}

int AxesModel::columnCount(const QModelIndex &parent) const
{
    if (infoIndex == -1)
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
            return temp.inverse.at(index.row());
        case 2:
            return temp.scale.at(index.row());
        case 3:
            return axisLabels.at(temp.axesMap.at(index.row()));
        }
    }
//    if (role == Qt::EditRole)
//    {
//        switch (column)
//        {
//        case 5: return static_cast<int>(deviceInfos.at(index.row()).output);
//        }
//    }
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
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled |//output axis
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
        deviceInfos[infoIndex].inverse[index.row()] = value.toInt();
        break;
//    case 5:
//        deviceInfos[index.row()].output = static_cast<OutputType::Output>(value.toInt());
//        break;
    default:
        return false;
    }
    DeviceConfig::clearConfiguredDevices();
    DeviceConfig::writeConfiguredDevices(this->deviceInfos);

    emit dataChanged(index, index);
    return true;
}

void AxesModel::selectionChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
    if (current.row() == infoIndex)
        return;
    this->beginResetModel();
    infoIndex = current.row();
    this->endResetModel();
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

void AxesView::openEditors()
{
    this->openPersistentEditor(this->model()->index(0, 1));
    this->openPersistentEditor(this->model()->index(1, 1));
    this->openPersistentEditor(this->model()->index(2, 1));
    this->openPersistentEditor(this->model()->index(3, 1));
    this->openPersistentEditor(this->model()->index(4, 1));
    this->openPersistentEditor(this->model()->index(5, 1));
}
