#include "otherequipmentregisterdialog.h"
#include "ui_otherequipmentregisterdialog.h"
#include <QMessageBox>

Q_DECLARE_METATYPE(otherEquipment)

otherEquipmentRegisterDialog::otherEquipmentRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::otherEquipmentRegisterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->equipmentShowTableWidget->setColumnCount(2);
    this->ui->equipmentShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Equipment ID") << tr("Equipment Name"));

    this->ui->equipmentShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->equipmentShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->equipmentShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QHeaderView *header = this->ui->equipmentShowTableWidget->horizontalHeader();
    header->setSortIndicator(0, Qt::AscendingOrder);
    header->setSortIndicatorShown(true);
    header->setClickable(true);
    connect(header, SIGNAL(sectionClicked(int)), this->ui->equipmentShowTableWidget, SLOT(sortByColumn(int)));

    connect(this->ui->insertPushButton, SIGNAL(clicked()), this, SLOT(otherEquipmentRegister()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(otherEquipmentRemove()));
}

otherEquipmentRegisterDialog::~otherEquipmentRegisterDialog()
{
    delete ui;
}

void otherEquipmentRegisterDialog::bindOtherEquipmentTable(QHash<QString, otherEquipment> *table)
{
    this->equipmentTable = table;

    QList<otherEquipment> equipmentList = this->equipmentTable->values();
    for(int i = 0; i < equipmentList.length(); i++)
    {
        this->insertEquipmentRow(equipmentList.value(i));
    }
}

void otherEquipmentRegisterDialog::otherEquipmentRegister()
{
    QString equipmentId = this->ui->equipmentIdLineEdit->text();

    if(equipmentId.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Input the Equipment ID Please"));
        return;
    }

    QRegExp rx("^[0-9]+$");
    if(rx.exactMatch(equipmentId) == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Equipment ID Format Invalid"));
        return;
    }

    if(this->equipmentTable->contains(equipmentId))
    {
        QMessageBox::warning(this, tr("Error"), tr("The Equipment Id has Contained"));
        return;
    }

    QString equipmentName = this->ui->equipmentNameLineEdit->text();


    otherEquipment equipment;

    equipment.setEquipmentId(equipmentId);
    equipment.setEquipmentName(equipmentName);

    emit this->insertOtherEquipment(equipment);
    this->equipmentTable->insert(equipmentId, equipment);
    this->insertEquipmentRow(equipment);

}

void otherEquipmentRegisterDialog::otherEquipmentRemove()
{
    int curRow = this->ui->equipmentShowTableWidget->currentRow();
    if(curRow == -1)
        return;

    QTableWidgetItem *equipmentItem = this->ui->equipmentShowTableWidget->item(curRow, 0);
    otherEquipment equipment = equipmentItem->data(Qt::UserRole).value<otherEquipment>();

    this->equipmentTable->remove(equipment.getEquipmentId());
    this->ui->equipmentShowTableWidget->removeRow(curRow);
    emit this->removeOtherEquipment(equipment);

}

void otherEquipmentRegisterDialog::insertEquipmentRow(otherEquipment equipment)
{
    int curRow = this->ui->equipmentShowTableWidget->rowCount();
    this->ui->equipmentShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *equipmentItem = new QTableWidgetItem(equipment.getEquipmentId());
    QVariant bindData;
    bindData.setValue(equipment);
    equipmentItem->setData(Qt::UserRole, bindData);

    this->ui->equipmentShowTableWidget->setItem(curRow, 0, equipmentItem);
    this->ui->equipmentShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(equipment.getEquipmentName()));
}
