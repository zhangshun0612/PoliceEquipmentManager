#include "policestaffregesterdialog.h"
#include "ui_policestaffregesterdialog.h"

#include <QMessageBox>
#include <QRegExp>
Q_DECLARE_METATYPE(policeStaff)
policeStaffRegesterDialog::policeStaffRegesterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::policeStaffRegesterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->policeStaffShowTableWidget->setColumnCount(3);
    this->ui->policeStaffShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Police Number") << tr("PoliceStaff Name") << tr("Department Name"));
    this->ui->policeStaffShowTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(this->ui->policeStaffShowTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(policeStaffNameItemEditting(QTableWidgetItem*)));
    connect(this->ui->policeStaffShowTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(policeStaffNameItemEditted(QTableWidgetItem*)));
    connect(this->ui->insertPushButton, SIGNAL(clicked()), this, SLOT(insertPushButtonClicked()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(removePushButtonClicked()));

}

policeStaffRegesterDialog::~policeStaffRegesterDialog()
{
    delete ui;
}

void policeStaffRegesterDialog::setPoliceStaffList(QList<policeStaff> *list)
{
    this->policeStaffList = list;
    this->ui->policeStaffShowTableWidget->setRowCount(this->policeStaffList->length());

    for(int i = 0 ; i < this->policeStaffList->length(); i++)
    {
        policeStaff policeOfficer = this->policeStaffList->value(i);
        QTableWidgetItem *policeNumberItem = new QTableWidgetItem(policeOfficer.getPoliceNumber());
        QVariant bindData;
        bindData.setValue(policeOfficer);
        policeNumberItem->setData(Qt::UserRole, bindData);
        policeNumberItem->setFlags(policeNumberItem->flags() & (~Qt::ItemIsEditable));

        this->ui->policeStaffShowTableWidget->setItem(i, 0 , policeNumberItem);
        this->ui->policeStaffShowTableWidget->setItem(i, 1, new QTableWidgetItem(policeOfficer.getStaffName()));
        this->ui->policeStaffShowTableWidget->setItem(i, 2, new QTableWidgetItem(policeOfficer.getDepartmentName()));
    }
}

void policeStaffRegesterDialog::insertPushButtonClicked()
{
    QString policeNumber = this->ui->policeNumberInputLineEdit->text().trimmed().replace(" ", "");
    if(policeNumber.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Input the Police Number"));
        return;
    }

    QRegExp rx("^[A-Z0-9]+$");
    if(rx.exactMatch(policeNumber) == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Police Number Format Invalid"));
        return;
    }

    QString policeName = this->ui->policeStaffNameInputLineEdit->text().trimmed().replace(" ", "");
    if(policeName.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Input the Police Staff's Name"));
        return;
    }

    QString departmentName = this->ui->departmentComboBox->currentText();

    policeStaff policeOfficer;
    policeOfficer.setPoliceNumber(policeNumber);
    policeOfficer.setStaffName(policeName);
    policeOfficer.setDepartmentName(departmentName);

    if(this->policeStaffList->contains(policeOfficer) == false)
    {
        emit this->insertPoliceStaff(policeOfficer);
        this->policeStaffList->append(policeOfficer);

        this->ui->policeStaffShowTableWidget->setRowCount(this->policeStaffList->length());
        int curRow = this->policeStaffList->length() - 1;
        QTableWidgetItem *policeNumberItem = new QTableWidgetItem(policeOfficer.getPoliceNumber());
        QVariant bindData;
        bindData.setValue(policeOfficer);
        policeNumberItem->setData(Qt::UserRole, bindData);
        policeNumberItem->setFlags(policeNumberItem->flags() & (~Qt::ItemIsEditable));

        this->ui->policeStaffShowTableWidget->setItem(curRow, 0 , policeNumberItem);
        this->ui->policeStaffShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(policeOfficer.getStaffName()));
        this->ui->policeStaffShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(policeOfficer.getDepartmentName()));
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("The Police Number Has contained"));
        return;
    }

}

void policeStaffRegesterDialog::removePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->policeStaffShowTableWidget->currentItem();

    int curRow = this->ui->policeStaffShowTableWidget->row(item);

    if(curRow == -1)
        return;

    QTableWidgetItem *policeNumberItem = this->ui->policeStaffShowTableWidget->item(curRow, 0);
    policeStaff policeOfficer = policeNumberItem->data(Qt::UserRole).value<policeStaff>();

    emit this->removePoliceStaff(policeOfficer);
    this->policeStaffList->removeOne(policeOfficer);

    this->ui->policeStaffShowTableWidget->removeRow(curRow);
}

void policeStaffRegesterDialog::policeStaffNameItemEditting(QTableWidgetItem *item)
{
    this->valueBeforeEditting = item->text();
    this->edittingItem = item;
}

void policeStaffRegesterDialog::policeStaffNameItemEditted(QTableWidgetItem *item)
{
   if(this->edittingItem != item)
       return;

   QString newStaffName = item->text().trimmed().replace(" ", "");
   if(newStaffName.isEmpty())
   {
       QMessageBox::warning(this, tr("Error"), tr("Please Input The Name"));
       item->setText(this->valueBeforeEditting);
       return;
   }

   int row = this->ui->policeStaffShowTableWidget->row(item);
   policeStaff policeOfficer = this->ui->policeStaffShowTableWidget->item(row, 0)->data(Qt::UserRole).value<policeStaff>();
   int index = this->policeStaffList->indexOf(policeOfficer);
   policeOfficer.setStaffName(newStaffName);
   if(index != -1)
   {
       this->policeStaffList->replace(index, policeOfficer);
       emit this->updatePoliceStaff(policeOfficer);
   }


}
