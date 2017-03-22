#include "firearmmanagerregisterdialog.h"
#include "ui_firearmmanagerregisterdialog.h"

Q_DECLARE_METATYPE(policeStaff)
firearmManagerRegisterDialog::firearmManagerRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firearmManagerRegisterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->firearmManagerShowTableWidget->setColumnCount(3);
    this->ui->firearmManagerShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Police Number") << tr("PoliceStaff Name") << tr("Department Name"));
    this->ui->firearmManagerShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->firearmManagerShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->firearmManagerShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this->ui->insertPushButton, SIGNAL(clicked()), this, SLOT(showPoliceStaffCanBeManager()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(removeFirearmManager()));
}

firearmManagerRegisterDialog::~firearmManagerRegisterDialog()
{
    delete ui;
}

void firearmManagerRegisterDialog::setPoliceStaffList(QList<policeStaff> *list)
{
    this->policeStaffList = list;

    for(int i = 0 ; i < list->length(); i++)
    {
        if(this->policeStaffList->value(i).isFirearmManager())
        {
            managerList.append(this->policeStaffList->value(i));
        }
    }

    this->ui->firearmManagerShowTableWidget->setRowCount(managerList.length());

    for(int i = 0 ; i < managerList.length(); i++)
    {
        policeStaff policeOfficer = managerList.value(i);
        QTableWidgetItem *policeNumberItem = new QTableWidgetItem(policeOfficer.getPoliceNumber());
        QVariant bindData;
        bindData.setValue(policeOfficer);
        policeNumberItem->setData(Qt::UserRole, bindData);
        policeNumberItem->setFlags(policeNumberItem->flags() & (~Qt::ItemIsEditable));

        this->ui->firearmManagerShowTableWidget->setItem(i, 0 , policeNumberItem);
        this->ui->firearmManagerShowTableWidget->setItem(i, 1, new QTableWidgetItem(policeOfficer.getStaffName()));
        this->ui->firearmManagerShowTableWidget->setItem(i, 2, new QTableWidgetItem(policeOfficer.getDepartmentName()));
    }
}

void firearmManagerRegisterDialog::showPoliceStaffCanBeManager()
{
    QList<policeStaff> list;
    for(int i = 0 ; i < this->policeStaffList->length(); i++)
    {
        if(this->policeStaffList->value(i).isFirearmManager() == false)
        {
            list.append(this->policeStaffList->value(i));
        }
    }

    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(list);
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        policeStaff manager = dialog->getChoosenOne();
        manager.setFirearmManager(true);
        int index = this->policeStaffList->indexOf(manager);
        if(index != -1)
        {
            this->policeStaffList->replace(index, manager);
            managerList.append(manager);

            emit this->updatePoliceStaffisManagerStatus(manager);
        }

        this->ui->firearmManagerShowTableWidget->setRowCount(managerList.length());
        int curRow = this->managerList.length() - 1;
        QTableWidgetItem *policeNumberItem = new QTableWidgetItem(manager.getPoliceNumber());
        QVariant bindData;
        bindData.setValue(manager);

        this->ui->firearmManagerShowTableWidget->setItem(curRow, 0 , policeNumberItem);
        this->ui->firearmManagerShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(manager.getStaffName()));

    }
    dialog->close();
    delete dialog;

}

void firearmManagerRegisterDialog::removeFirearmManager()
{
    QTableWidgetItem *item = this->ui->firearmManagerShowTableWidget->currentItem();

    int curRow = this->ui->firearmManagerShowTableWidget->row(item);

    if(curRow == -1)
        return;

    QTableWidgetItem *policeNumberItem = this->ui->firearmManagerShowTableWidget->item(curRow, 0);
    policeStaff manager = policeNumberItem->data(Qt::UserRole).value<policeStaff>();
    manager.setFirearmManager(false);
    int index = this->policeStaffList->indexOf(manager);
    if(index != -1)
    {
        this->policeStaffList->replace(index, manager);
        managerList.removeOne(manager);

        emit this->updatePoliceStaffisManagerStatus(manager);
    }

    this->ui->firearmManagerShowTableWidget->removeRow(curRow);
}
