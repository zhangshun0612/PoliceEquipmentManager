#include "otherequipmentchoosedialog.h"
#include "ui_otherequipmentchoosedialog.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(otherEquipment)
otherEquipmentChooseDialog::otherEquipmentChooseDialog(enum otherEquipmentDialogType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::otherEquipmentChooseDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->otherEquipmentShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->otherEquipmentShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHeaderView *header = this->ui->otherEquipmentShowTableWidget->horizontalHeader();

    header->setSortIndicator(0, Qt::AscendingOrder);
    header->setSortIndicatorShown(true);
    header->setClickable(true);

    connect(header, SIGNAL(sectionClicked(int)), this->ui->otherEquipmentShowTableWidget, SLOT(sortByColumn(int)));

    dialogType = type;
    if(type == LendInfo)
    {
        this->ui->otherEquipmentShowTableWidget->setColumnCount(2);
        this->ui->otherEquipmentShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Other Equipment ID") << tr("Other Equipment Name"));
    }
    else
    {
        this->ui->otherEquipmentShowTableWidget->setColumnCount(4);
        this->ui->otherEquipmentShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Other Equipment ID") << tr("Other Equipment Name")
                                                                    << tr("LastUser Police Name Number") << tr("LastUser Name"));
    }

    this->ui->otherEquipmentShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    connect(this->ui->choosePushButton, SIGNAL(clicked()), this, SLOT(choosePushButtonClicked()));
}

otherEquipmentChooseDialog::~otherEquipmentChooseDialog()
{
    delete ui;
}

void otherEquipmentChooseDialog::setOtherEquipmentTable(QList<otherEquipment> list)
{
    this->ui->otherEquipmentShowTableWidget->setRowCount(list.length());

    for(int i = 0 ; i < list.length(); i++)
    {
        otherEquipment equipment = list.value(i);
        QTableWidgetItem *equipmentIdItem = new QTableWidgetItem(equipment.getEquipmentId());
        QVariant bindData;
        bindData.setValue(equipment);
        equipmentIdItem->setData(Qt::UserRole, bindData);
        this->ui->otherEquipmentShowTableWidget->setItem(i, 0, equipmentIdItem);
        this->ui->otherEquipmentShowTableWidget->setItem(i, 1, new QTableWidgetItem(equipment.getEquipmentName()));

        if(dialogType == LendInfo)
        {

        }
        else
        {
            this->ui->otherEquipmentShowTableWidget->setItem(i, 2, new QTableWidgetItem(equipment.getEquipmentLastUser().getPoliceNumber()));
            this->ui->otherEquipmentShowTableWidget->setItem(i, 3, new QTableWidgetItem(equipment.getEquipmentLastUser().getStaffName()));
        }
    }
}

otherEquipment otherEquipmentChooseDialog::getChoosenOne()
{
    return this->choosenOne;
}

void otherEquipmentChooseDialog::choosePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->otherEquipmentShowTableWidget->currentItem();

    int curRow = this->ui->otherEquipmentShowTableWidget->row(item);

    if(curRow == -1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose a item First"));
        return;
    }
    QTableWidgetItem *equipmentIdItem = this->ui->otherEquipmentShowTableWidget->item(curRow, 0);
    otherEquipment equipment = equipmentIdItem->data(Qt::UserRole).value<otherEquipment>();

    this->choosenOne = equipment;

    this->accept();
}
