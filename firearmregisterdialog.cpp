#include "firearmregisterdialog.h"
#include "ui_firearmregisterdialog.h"

#include "policestaffchoosedialog.h"

#include <QMessageBox>
#include <QRegExp>
#include <QDebug>

Q_DECLARE_METATYPE(fireArmClass)
Q_DECLARE_METATYPE(fireArm)

fireArmRegisterDialog::fireArmRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fireArmRegisterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->firearmLastUserChoosen = false;
    this->firearmManagerChoosen = false;

    this->fireArmModelTable = new QHash<QString, fireArmClass>();
    this->policeOfficerTable = new QHash<QString,  policeStaff>();

    this->ui->fireArmShowTableWidget->setColumnCount(8);
    this->ui->fireArmShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Fire Arm Number") << tr("Fire Arm Model")
                                                                << tr("Manager Police Number") << tr("Manager Police Name")
                                                                << tr("LastUser Police Number") << tr("LastUser Police Name")
                                                                << tr("Mage Number") << tr("Last Maintenance Date"));
    this->ui->fireArmShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->fireArmShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->fireArmShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);


    QHeaderView *header = this->ui->fireArmShowTableWidget->horizontalHeader();
    header->setSortIndicator(0, Qt::AscendingOrder);
    header->setSortIndicatorShown(true);
    header->setClickable(true);
    connect(header, SIGNAL(sectionClicked(int)), this->ui->fireArmShowTableWidget, SLOT(sortByColumn(int)));

    this->ui->lastMaintaneceDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(this->ui->firearmManagerLineEdit, SIGNAL(lineEditPressed()), this, SLOT(fireArmManagerChoose()));
    connect(this->ui->firearmLastUserLineEdit, SIGNAL(lineEditPressed()), this, SLOT(fireArmLastUserChoose()));
    connect(this->ui->registerPushButton, SIGNAL(clicked()), this, SLOT(fireArmRegister()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(fireArmRemove()));
}

fireArmRegisterDialog::~fireArmRegisterDialog()
{
    delete this->fireArmModelTable;
    delete this->policeOfficerTable;
    delete ui;
}

void fireArmRegisterDialog::setupFireArmModelTable(QList<fireArmClass> *list)
{
    for(int i = 0 ; i < list->length(); i++)
    {
        fireArmClass fireArmModel = list->value(i);
        QString fireArmModelName =fireArmModel.getFireArmClassName();
        QVariant bindData;
        bindData.setValue(fireArmModel);
        this->ui->fireArmModelListComboBox->insertItem(i, fireArmModelName, bindData);
        this->fireArmModelTable->insert(fireArmModelName, list->value(i));
    }
}

void fireArmRegisterDialog::setupPoliceOfficerTable(QList<policeStaff> *list)
{
    for(int i = 0 ; i < list->length(); i++)
    {
        policeStaff policeOfficer = list->value(i);
        QString policeOfficerNumber = policeOfficer.getPoliceNumber();

        this->policeOfficerTable->insert(policeOfficerNumber, policeOfficer);
    }
}

void fireArmRegisterDialog::bindFireArmTable(QHash<QString, fireArm> *table)
{
    this->fireArmTable = table;
    QList<fireArm> gunList = table->values();
    for(int i = 0 ; i < gunList.length(); i++)
    {
        this->insertFirearmRow(gunList.value(i));
    }
    this->ui->fireArmShowTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void fireArmRegisterDialog::fireArmManagerChoose()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    QList<policeStaff> stafflist = this->policeOfficerTable->values();
    QList<policeStaff> managerlist;

    for(int i = 0 ; i < stafflist.length(); i++)
    {
        if(stafflist.value(i).isFirearmManager())
        {
            managerlist.append(stafflist.value(i));
        }
    }

    dialog->setPoliceOfficerTable(managerlist);
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmManager = dialog->getChoosenOne();
        this->firearmManagerChoosen = true;

        this->ui->firearmManagerLineEdit->setText(this->firearmManager.getPoliceNumber() + ": " + this->firearmManager.getStaffName());
    }

    dialog->close();
    delete dialog;
}

void fireArmRegisterDialog::fireArmLastUserChoose()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(this->policeOfficerTable->values());
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmLastUser = dialog->getChoosenOne();
        this->firearmLastUserChoosen = true;

        this->ui->firearmLastUserLineEdit->setText(this->firearmLastUser.getPoliceNumber() + ": " + this->firearmLastUser.getStaffName());
    }

    dialog->close();
    delete dialog;
}

void fireArmRegisterDialog::fireArmRegister()
{
    QString firearmNumber = this->ui->firearmNumberLineEdit->text().trimmed().replace(" ", "");
    if(firearmNumber.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Input the Firearm Number Please"));
        return;
    }

    QRegExp rx("^[0-9]+$");
    if(rx.exactMatch(firearmNumber) == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Firearm Number Format Invalid"));
        return;
    }

    if(this->fireArmTable->contains(firearmNumber))
    {
        QMessageBox::warning(this, tr("Error"), tr("The FireArm Number has Contained"));
        return;
    }

    int magaNumber = this->ui->magaNumberSpinBox->value();

    fireArmClass firearmModel;
    int index = this->ui->fireArmModelListComboBox->currentIndex();
    if(index == -1)
    {
        QMessageBox::warning(this, tr("Error"), tr("No valid Firearm Model"));
        return;
    }
    firearmModel = this->ui->fireArmModelListComboBox->itemData(index, Qt::UserRole).value<fireArmClass>();
    qDebug() << firearmModel.getFireArmClassName();
    if(this->firearmManagerChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Choose the Firearm Manager"));
        return;
    }
/*
    if(this->firearmLastUserChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Choose the Firearm LastUser"));
        return;
    }
*/
    QDateTime lastMaintanenceDate = this->ui->lastMaintaneceDateDateTimeEdit->dateTime();

    fireArm gun;
    gun.setFireArmNumber(firearmNumber);
    gun.setMagaNumber(magaNumber);
    gun.setFireArmModel(firearmModel);
    gun.setFireArmManager(firearmManager);
    gun.setFireArmLastUser(firearmLastUser);
    gun.setLastMaintenanceDate(lastMaintanenceDate);

    emit this->insertFirearm(&gun);
    this->fireArmTable->insert(firearmNumber, gun);
    this->insertFirearmRow(gun);
}

void fireArmRegisterDialog::fireArmRemove()
{
    int curRow = this->ui->fireArmShowTableWidget->currentRow();
    if(curRow == -1)
        return;

    QTableWidgetItem *firearmNumberItem = this->ui->fireArmShowTableWidget->item(curRow, 0);
    fireArm gun = firearmNumberItem->data(Qt::UserRole).value<fireArm>();

    this->fireArmTable->remove(gun.getFireArmNumber());
    this->ui->fireArmShowTableWidget->removeRow(curRow);
    emit this->removeFirearm(&gun);
}

void fireArmRegisterDialog::insertFirearmRow(fireArm gun)
{

    int curRow = this->ui->fireArmShowTableWidget->rowCount();
    this->ui->fireArmShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *firearmNumberItem = new QTableWidgetItem(gun.getFireArmNumber());
    QVariant bindData;
    bindData.setValue(gun);
    firearmNumberItem->setData(Qt::UserRole, bindData);

    this->ui->fireArmShowTableWidget->setItem(curRow, 0, firearmNumberItem);
    this->ui->fireArmShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(gun.getFireArmModel().getFireArmClassName()));
    this->ui->fireArmShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(gun.getFireArmManager().getPoliceNumber()));
    this->ui->fireArmShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(gun.getFireArmManager().getStaffName()));
    this->ui->fireArmShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(gun.getFireArmLastUser().getPoliceNumber()));
    this->ui->fireArmShowTableWidget->setItem(curRow, 5, new QTableWidgetItem(gun.getFireArmLastUser().getStaffName()));
    this->ui->fireArmShowTableWidget->setItem(curRow, 6, new QTableWidgetItem(QString::number(gun.getMagaNumber())));
    this->ui->fireArmShowTableWidget->setItem(curRow, 7, new QTableWidgetItem(gun.getLastMaintenanceDate().toString("yyyy-MM-dd hh:mm:ss")));
}
