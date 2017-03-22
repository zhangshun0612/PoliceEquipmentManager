#include "policeequipmentmanager.h"
#include "ui_policeequipmentmanager.h"

#include "firearmclassregisterdialog.h"
#include "policestaffregesterdialog.h"
#include "firearmregisterdialog.h"
#include "firearmchoosedialog.h"
#include "policestaffchoosedialog.h"
#include "firearmmanagerregisterdialog.h"
#include "vehicleregisterdialog.h"
#include "recordinquirydialog.h"
#include "vehiclechoosedialog.h"
#include "otherequipmentregisterdialog.h"
#include "otherequipmentchoosedialog.h"
#include "logindialog.h"
#include "passwordchangedialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QFileInfo>

Q_DECLARE_METATYPE(fireArmClass)
Q_DECLARE_METATYPE(fireArm)
Q_DECLARE_METATYPE(vehicle)
Q_DECLARE_METATYPE(otherEquipment)

PoliceEquipmentManager::PoliceEquipmentManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PoliceEquipmentManager)
{
    ui->setupUi(this);
    this->firearmToLendChoosen = false;
    this->firearmLenderChoosen = false;
    this->firearmManagerChoosen = false;
    this->firearmToReturnChoosen = false;
    this->firearmToMaintenanceChoosen = false;


    this->fireArmClasses = new QList<fireArmClass>();
    this->policeStaffs = new QList<policeStaff>();
    this->fireArms = new QHash<QString, fireArm>();
    this->vehicles = new QHash<QString, vehicle>();
    this->equipments = new QHash<QString, otherEquipment>();

    this->DBManager = new DataBaseManager(this);
    this->firearmMaintenanceCheckTimer = new QTimer();
    this->firearmMaintenanceCheckTimer->setInterval(1000 * 3600);

    this->DBManager->initTables();
    this->fireArmClasses->append(this->DBManager->getFireArmClassesList());
    this->policeStaffs->append(this->DBManager->getPoliceStaffsList());
    *fireArms = this->DBManager->getFireArmTable();
    *vehicles = this->DBManager->getVehicleTable();
    *equipments = this->DBManager->getOtherEquipmentTable();

    this->managerPassword = this->DBManager->getPassword();
    qDebug() << this->managerPassword;

    QStringList firearmHeaderLabels;
    firearmHeaderLabels << tr("Firearm Number") << tr("Firearm Model")
                    << tr("LastUser Police Number") << tr("LastUser Police Name")
                       <<tr("Last Use Date") << tr("Last Maintenance Date")
                         << tr("Is need maintenance")<<tr("Is in store");

    this->ui->firearmsShowTableWidget->setColumnCount(firearmHeaderLabels.length());
    this->ui->firearmsShowTableWidget->setHorizontalHeaderLabels(firearmHeaderLabels);
    this->ui->firearmsShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->firearmsShowTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    this->ui->firearmsShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->firearmsShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->ui->maintenanceDateDateTimeEdit->setDateTime(QDateTime::currentDateTime());
    this->tableContentShowType = All;
    this->refreshFireArmsTable();

    this->ui->firearmsShowTableWidget->sortByColumn(0, Qt::AscendingOrder);
    QHeaderView *firearmheader = this->ui->firearmsShowTableWidget->horizontalHeader();

    firearmheader->setSortIndicator(0, Qt::AscendingOrder);
    firearmheader->setSortIndicatorShown(true);
    firearmheader->setClickable(true);

    connect(firearmheader, SIGNAL(sectionClicked(int)), this->ui->firearmsShowTableWidget, SLOT(sortByColumn(int)));

    QStringList vehicleHeaderLabels;
    vehicleHeaderLabels << tr("Plate Number") << tr("Car Type")
                          << tr("Last Driver Police Number") << tr("Last Driver Name")
                       <<tr("Last Use Date") << tr("Last Wash Date")
                         << tr("Is need Wash")<<tr("Is in Parking");
    this->ui->vehicleShowTableWidget->setColumnCount(vehicleHeaderLabels.length());
    this->ui->vehicleShowTableWidget->setHorizontalHeaderLabels(vehicleHeaderLabels);
    this->ui->vehicleShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->vehicleShowTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    this->ui->vehicleShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->vehicleShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->refreshVehicleTable();

    this->ui->vehicleShowTableWidget->sortByColumn(0, Qt::AscendingOrder);
    QHeaderView *vehicleheader = this->ui->vehicleShowTableWidget->horizontalHeader();

    vehicleheader->setSortIndicator(0, Qt::AscendingOrder);
    vehicleheader->setSortIndicatorShown(true);
    vehicleheader->setClickable(true);

    connect(vehicleheader, SIGNAL(sectionClicked(int)), this->ui->vehicleShowTableWidget, SLOT(sortByColumn(int)));


    QStringList otherEquipmentHeaderLabels;
    otherEquipmentHeaderLabels << tr("Equipment ID") << tr("Equipment Name")
                            << tr("LastUser Police Number") << tr("LastUser Police Name")
                            << tr("Last Use Date") << tr("Is in equipment store");

    this->ui->otherEquipmentShowTableWidget->setColumnCount(otherEquipmentHeaderLabels.length());
    this->ui->otherEquipmentShowTableWidget->setHorizontalHeaderLabels(otherEquipmentHeaderLabels);
    this->ui->otherEquipmentShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->otherEquipmentShowTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    this->ui->otherEquipmentShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->otherEquipmentShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->refreshOtherEquipmentTable();

    this->ui->otherEquipmentShowTableWidget->sortByColumn(0, Qt::AscendingOrder);
    QHeaderView *otherequipmentheader = this->ui->otherEquipmentShowTableWidget->horizontalHeader();

    otherequipmentheader->setSortIndicator(0, Qt::AscendingOrder);
    otherequipmentheader->setSortIndicatorShown(true);
    otherequipmentheader->setClickable(true);

    connect(otherequipmentheader, SIGNAL(sectionClicked(int)), this->ui->otherEquipmentShowTableWidget, SLOT(sortByColumn(int)));

    connect(this->ui->actionShowFirearm, SIGNAL(triggered()), this, SLOT(showFirearmMainWidget()));
    connect(this->ui->actionShowVehicle, SIGNAL(triggered()), this, SLOT(showVehicleMainWidget()));
    connect(this->ui->actionShowOtherEquipment, SIGNAL(triggered()), this, SLOT(showOtherEquipmentMainWidget()));

    connect(this->ui->actionRegister_Firearm_Class, SIGNAL(triggered()), this, SLOT(showFirearmClassRegisterDialog()));
    connect(this->ui->actionRegister_Police_Staff, SIGNAL(triggered()), this, SLOT(showPoliceStaffRegisterDialog()));
    connect(this->ui->actionRegister_Manager, SIGNAL(triggered()), this, SLOT(showPoliceManagerRegisterDialog()));
    connect(this->ui->actionRegister_Firearm, SIGNAL(triggered()), this, SLOT(showFireArmRegisterDialog()));
    connect(this->ui->actionRegister_Vehicle, SIGNAL(triggered()), this, SLOT(showVehicleRegisterDialog()));
    connect(this->ui->actionRegister_Other_Equipment, SIGNAL(triggered()), this, SLOT(showOtherEquipmentRegisterDialog()));

    connect(this->ui->actionRefresh, SIGNAL(triggered()), this, SLOT(checkFirearmMaintenaceStatus()));
    connect(this->ui->actionRecord_Inquiry, SIGNAL(triggered()), this, SLOT(showInquiryRecordDialog()));
    connect(this->ui->actionModify_Manager_Password, SIGNAL(triggered()), this, SLOT(showPasswordChangeDialog()));

    connect(this->ui->gunToLendLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showFirearmToLend()));
    connect(this->ui->firearmLenderLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showPoliceStaffToLendGun()));
    connect(this->ui->firearmManagerLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showFirearmManager()));
    connect(this->ui->lendGunPushButton, SIGNAL(clicked()), this, SLOT(lendGun()));

    connect(this->ui->gunToReturnLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showFirearmToReturn()));
    connect(this->ui->returnGunPushButton, SIGNAL(clicked()), this, SLOT(returnGun()));

    connect(this->ui->gunToMaintenanceLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showFirearmToMaintenance()));
    connect(this->ui->maintenanceGunPushButton, SIGNAL(clicked()), this, SLOT(maintenanceGun()));

    connect(this->ui->equipmentToLendLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showOtherEquipmentToLend()));
    connect(this->ui->equipmentLenderLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showPoliceStaffToLendOtherEquipment()));
    connect(this->ui->equipmentManagerLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showOtherEquipmentManager()));
    connect(this->ui->equipmentToReturnLineEdit, SIGNAL(lineEditPressed()), this, SLOT(showOtherEquipmentToReturn()));

    connect(this->ui->allShowRadioButton, SIGNAL(clicked()), this, SLOT(fireArmShowTableContentsToShowChanged()));
    connect(this->ui->instoreShowRadioButton, SIGNAL(clicked()), this, SLOT(fireArmShowTableContentsToShowChanged()));
    connect(this->ui->outstoreShowRadioButton, SIGNAL(clicked()), this, SLOT(fireArmShowTableContentsToShowChanged()));
    connect(this->ui->needMaintenaceShowRadioButton, SIGNAL(clicked()), this, SLOT(fireArmShowTableContentsToShowChanged()));
    connect(this->ui->notNeedMaintenanceShowRadioButton, SIGNAL(clicked()), this, SLOT(fireArmShowTableContentsToShowChanged()));

    connect(this->firearmMaintenanceCheckTimer, SIGNAL(timeout()), this, SLOT(checkFirearmMaintenaceStatus()));

    connect(this->ui->lendCarPushButton, SIGNAL(clicked()), this, SLOT(lendCar()));
    connect(this->ui->returnCarPushButton, SIGNAL(clicked()), this, SLOT(returnCar()));
    connect(this->ui->washCarPushButton, SIGNAL(clicked()), this, SLOT(washCar()));

    connect(this->ui->lendEquipmentPushButton, SIGNAL(clicked()), this, SLOT(lendOtherEquipment()));
    connect(this->ui->returnEquipmentPushButton, SIGNAL(clicked()), this, SLOT(returnOtherEquipment()));

    this->firearmMaintenanceCheckTimer->start();
}

PoliceEquipmentManager::~PoliceEquipmentManager()
{
    delete ui;
    delete this->equipments;
    delete this->vehicles;
    delete this->fireArms;
    delete this->fireArmClasses;
    delete this->policeStaffs;
    delete this->DBManager;
    delete this->firearmMaintenanceCheckTimer;
}

void PoliceEquipmentManager::showFirearmMainWidget()
{
    this->ui->actionShowFirearm->setChecked(true);
    this->ui->actionShowVehicle->setChecked(false);
    this->ui->actionShowOtherEquipment->setChecked(false);
    this->ui->stackedWidget->setCurrentIndex(0);
}

void PoliceEquipmentManager::showVehicleMainWidget()
{
    this->ui->actionShowFirearm->setChecked(false);
    this->ui->actionShowVehicle->setChecked(true);
    this->ui->actionShowOtherEquipment->setChecked(false);
    this->ui->stackedWidget->setCurrentIndex(1);
}

void PoliceEquipmentManager::showOtherEquipmentMainWidget()
{
    this->ui->actionShowFirearm->setChecked(false);
    this->ui->actionShowVehicle->setChecked(false);
    this->ui->actionShowOtherEquipment->setChecked(true);
    this->ui->stackedWidget->setCurrentIndex(2);
}

void PoliceEquipmentManager::showFirearmClassRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }

    fireArmClassRegisterDialog *dialog = new fireArmClassRegisterDialog(this);
    dialog->setFireArmClassList(this->fireArmClasses);
    dialog->setWindowFlags(Qt::Window);

    connect(dialog, SIGNAL(insertFireArmClass(fireArmClass)), this->DBManager, SLOT(insertFirearmClassTable(fireArmClass)));
    connect(dialog, SIGNAL(updateFireArmClass(fireArmClass)), this->DBManager, SLOT(updateFirearmClassTable(fireArmClass)));
    connect(dialog, SIGNAL(removeFireArmClass(fireArmClass)), this->DBManager, SLOT(removeFirearmClassTable(fireArmClass)));
    dialog->show();
    dialog->exec();


    dialog->close();
    delete dialog;

    this->refreshFireArmsTable();
}

void PoliceEquipmentManager::showPoliceStaffRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }

    policeStaffRegesterDialog *dialog = new policeStaffRegesterDialog(this);
    dialog->setPoliceStaffList(this->policeStaffs);

    connect(dialog, SIGNAL(insertPoliceStaff(policeStaff)), this->DBManager, SLOT(insertPoliceStaffTable(policeStaff)));
    connect(dialog, SIGNAL(updatePoliceStaff(policeStaff)), this->DBManager, SLOT(updatePoliceStaffNameTable(policeStaff)));
    connect(dialog, SIGNAL(removePoliceStaff(policeStaff)), this->DBManager, SLOT(removePoliceStaffTable(policeStaff)));

    dialog->show();
    dialog->exec();

    dialog->close();
    delete dialog;

    this->refreshFireArmsTable();
}

void PoliceEquipmentManager::showPoliceManagerRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }


    firearmManagerRegisterDialog *dialog = new firearmManagerRegisterDialog(this);
    dialog->setPoliceStaffList(this->policeStaffs);
    connect(dialog, SIGNAL(updatePoliceStaffisManagerStatus(policeStaff)), this->DBManager, SLOT(updatePoliceStaffIsFirearmManager(policeStaff)));

    dialog->show();
    dialog->exec();

    dialog->close();
    delete dialog;

    this->refreshFireArmsTable();
}

void PoliceEquipmentManager::showFireArmRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }

    fireArmRegisterDialog *dialog = new fireArmRegisterDialog(this);
    dialog->setupFireArmModelTable(this->fireArmClasses);
    dialog->setupPoliceOfficerTable(this->policeStaffs);
    dialog->bindFireArmTable(this->fireArms);
    connect(dialog, SIGNAL(insertFirearm(fireArm*)), this->DBManager, SLOT(insertFirearmTable(fireArm*)));
    connect(dialog, SIGNAL(removeFirearm(fireArm*)), this->DBManager, SLOT(removeFirearmTable(fireArm*)));
    dialog->show();
    dialog->exec();

    dialog->close();
    delete dialog;
    qDebug() << "refresh ";
    this->refreshFireArmsTable();
}

void PoliceEquipmentManager::showVehicleRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }

    vehicleRegisterDialog *dialog = new vehicleRegisterDialog(this);
    dialog->bindVehicleTable(this->vehicles);
    connect(dialog, SIGNAL(insertVehicle(vehicle)), this->DBManager, SLOT(insertVehicleTable(vehicle)));
    connect(dialog, SIGNAL(removeVehicle(vehicle)), this->DBManager, SLOT(removeVehicleTable(vehicle)));
    connect(dialog, SIGNAL(updateVehicle(vehicle)), this->DBManager, SLOT(updateVehicleTable(vehicle)));
    dialog->show();
    dialog->exec();

    dialog->close();
    delete dialog;
    this->refreshVehicleTable();
}

void PoliceEquipmentManager::showOtherEquipmentRegisterDialog()
{
    if(false == this->showLogin())
    {
        return;
    }

    otherEquipmentRegisterDialog *dialog = new otherEquipmentRegisterDialog(this);
    dialog->bindOtherEquipmentTable(this->equipments);

    connect(dialog, SIGNAL(insertOtherEquipment(otherEquipment)), this->DBManager, SLOT(insertOtherEquipment(otherEquipment)));
    connect(dialog, SIGNAL(removeOtherEquipment(otherEquipment)), this->DBManager, SLOT(removeOtherEquipment(otherEquipment)));

    dialog->show();
    dialog->exec();


    dialog->close();
    delete dialog;
    this->refreshOtherEquipmentTable();

}

void PoliceEquipmentManager::showInquiryRecordDialog()
{
    recordInquiryDialog *dialog = new recordInquiryDialog(this);
    dialog->show();
    dialog->exec();

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showPasswordChangeDialog()
{
    passwordChangeDialog *dialog = new passwordChangeDialog(this->managerPassword, this);

    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->managerPassword = dialog->getNewManagerPassword();
        this->DBManager->updatePasswordTable(this->managerPassword);
    }

    dialog->close();
    delete dialog;

}


void PoliceEquipmentManager::showFirearmToLend()
{
    QList<fireArm> firearmList = this->fireArms->values();
    QList<fireArm> firearmToLendList;

    for(int i = 0 ; i < firearmList.length(); i++)
    {
        if(firearmList.value(i).isInStore() == true)
        {
            firearmToLendList.append(firearmList.value(i));
        }
    }



    fireArmChooseDialog *dialog = new fireArmChooseDialog(fireArmChooseDialog::LendInfo, this);
    dialog->setFirearmTable(firearmToLendList);
    dialog->show();

    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmToLend = dialog->getChoosenOne();
        this->firearmToLendChoosen = true;

        this->ui->gunToLendLineEdit->setText(this->firearmToLend.getFireArmNumber() + " " + this->firearmToLend.getFireArmModel().getFireArmClassName());
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showPoliceStaffToLendGun()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(*(this->policeStaffs));
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmLender = dialog->getChoosenOne();
        this->firearmLenderChoosen = true;

        this->ui->firearmLenderLineEdit->setText(this->firearmLender.getPoliceNumber() + ": " + this->firearmLender.getStaffName());
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showFirearmManager()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(this->getFirearmManagerList());
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

void PoliceEquipmentManager::lendGun()
{
    if(this->firearmToLendChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the gun to lend"));
        return;
    }

    if(this->firearmLenderChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the police Officer who lend the gun"));
        return;
    }

    if(this->firearmManagerChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the police Officer who manage the gun"));
        return;
    }


    QDateTime curDateTime = QDateTime::currentDateTime();
    this->firearmToLend.lendThisFirearm(this->firearmLender,this->firearmManager ,curDateTime);

    this->fireArms->insert(this->firearmToLend.getFireArmNumber(), this->firearmToLend);

    this->DBManager->lendTheFirearm(this->firearmToLend);
    this->fireArmStatusChanged(this->firearmToLend, LendOut);
    this->ui->gunToLendLineEdit->setText("");
    this->firearmToLendChoosen = false;
}


void PoliceEquipmentManager::showFirearmToReturn()
{
    QList<fireArm> firearmList = this->fireArms->values();
    QList<fireArm> firearmToReturnList;

    for(int i = 0 ; i < firearmList.length(); i++)
    {
        if(firearmList.value(i).isInStore() == false)
        {
            firearmToReturnList.append(firearmList.value(i));
        }
    }



    fireArmChooseDialog *dialog = new fireArmChooseDialog(fireArmChooseDialog::ReturnInfo, this);
    dialog->setFirearmTable(firearmToReturnList);
    dialog->show();

    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmToReturn = dialog->getChoosenOne();
        this->firearmToReturnChoosen = true;

        this->ui->gunToReturnLineEdit->setText(this->firearmToReturn.getFireArmNumber() + " " + this->firearmToReturn.getFireArmModel().getFireArmClassName());
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::returnGun()
{
    if(this->firearmToReturnChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the gun to Return"));
        return;
    }

    this->firearmToReturn.returnThisFirearm();

    this->fireArms->insert(this->firearmToReturn.getFireArmNumber(), this->firearmToReturn);
    this->DBManager->returnTheFirearm(this->firearmToReturn);
    this->fireArmStatusChanged(this->firearmToReturn, ReturnBack);
    this->ui->gunToReturnLineEdit->setText("");
    this->firearmToReturnChoosen = false;
}

void PoliceEquipmentManager::showFirearmToMaintenance()
{
    QList<fireArm> firearmList = this->fireArms->values();

    fireArmChooseDialog *dialog = new fireArmChooseDialog(fireArmChooseDialog::MaintenanceInfo, this);
    dialog->setFirearmTable(firearmList);
    dialog->show();

    if(QDialog::Accepted == dialog->exec())
    {
        this->firearmToMaintenance = dialog->getChoosenOne();
        this->firearmToMaintenanceChoosen = true;

        this->ui->gunToMaintenanceLineEdit->setText(this->firearmToMaintenance.getFireArmNumber() + " " + this->firearmToMaintenance.getFireArmModel().getFireArmClassName());
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::maintenanceGun()
{
    if(this->firearmToMaintenanceChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the gun to Maintenance"));
        return;
    }


    if(this->firearmToMaintenance.getLastMaintenanceDate() > this->ui->maintenanceDateDateTimeEdit->dateTime())
    {
        QMessageBox::warning(this, tr("Error"), tr("Maintenance Time Error"));
        return;
    }

    this->firearmToMaintenance.maintenanceThisFirearm(this->ui->maintenanceDateDateTimeEdit->dateTime());
    this->fireArms->insert(this->firearmToMaintenance.getFireArmNumber(), this->firearmToMaintenance);
    this->fireArmStatusChanged(this->firearmToMaintenance, Maintenanced);
    this->DBManager->maintenanceTheFirearm(this->firearmToMaintenance, this->ui->maintenanceDateDateTimeEdit->dateTime());

    this->ui->gunToMaintenanceLineEdit->setText("");
    this->firearmToMaintenanceChoosen = false;
}

void PoliceEquipmentManager::checkFirearmMaintenaceStatus()
{
    QList<fireArm> fireArmList = this->fireArms->values();
    for(int i = 0 ; i < fireArmList.length(); i++)
    {
        int maintenanceDayInterval = fireArmList.value(i).getFireArmModel().getMaintenanceDayInterval();
        if(fireArmList.value(i).getLastMaintenanceDate().daysTo(QDateTime::currentDateTime()) > maintenanceDayInterval && fireArmList.value(i).isNeedMaintenance() == false)
        {
            fireArm choosenFirearm = fireArmList.value(i);
            choosenFirearm.setNeedMaintenanceStatus(true);
            this->fireArms->insert(choosenFirearm.getFireArmNumber(), choosenFirearm);
            //用时较多
            this->DBManager->remindMaintenanceTheFirearm(choosenFirearm.getFireArmNumber());
            this->fireArmStatusChanged(choosenFirearm, RemindToMaintenance);
        }
    }

}


void PoliceEquipmentManager::refreshFireArmsTable()
{
    this->fireArms->clear();
    *fireArms = this->DBManager->getFireArmTable();

    //用时较多
    this->checkFirearmMaintenaceStatus();

    this->refreshFirearrmsShowTable(this->tableContentShowType);

}

void PoliceEquipmentManager::refreshVehicleTable()
{
    this->vehicles->clear();

    *vehicles = this->DBManager->getVehicleTable();
    this->refreshVehicleShowTable();
}

void PoliceEquipmentManager::refreshOtherEquipmentTable()
{
    this->equipments->clear();

    *equipments = this->DBManager->getOtherEquipmentTable();
    this->refreshEquipmentShowTable();
}

void PoliceEquipmentManager::fireArmShowTableContentsToShowChanged()
{
    QObject *obj = QObject::sender();

    if(obj == this->ui->allShowRadioButton)
    {
        this->tableContentShowType = All;
    }
    else if(obj == this->ui->instoreShowRadioButton)
    {
        this->tableContentShowType = Instore;
    }
    else if(obj == this->ui->outstoreShowRadioButton)
    {
        this->tableContentShowType = Outstore;
    }
    else if(obj == this->ui->needMaintenaceShowRadioButton)
    {
        this->tableContentShowType = NeedMaintenance;
    }
    else if(obj == this->ui->notNeedMaintenanceShowRadioButton)
    {
        this->tableContentShowType = NotNeedMaintenance;
    }
    this->refreshFirearrmsShowTable(this->tableContentShowType);
}

void PoliceEquipmentManager::fireArmStatusChanged(fireArm gun, firearmStatusChangedBehavior beharvior)
{
    if(this->tableContentShowType == Instore)
    {
        switch (beharvior) {
        case LendOut:
            this->removeFirearmShowTableRow(gun);
            break;
        case ReturnBack:
            this->insertFirearmShowTableRow(gun);
            break;
        default:
            this->updateFirearmShowTableRow(gun);
            break;
        }
    }
    else if(this->tableContentShowType == Outstore)
    {
        switch (beharvior) {
        case LendOut:
            this->insertFirearmShowTableRow(gun);
            break;
        case ReturnBack:
            this->removeFirearmShowTableRow(gun);
            break;
        default:
            this->updateFirearmShowTableRow(gun);
            break;
        }
    }
    else if(this->tableContentShowType == NeedMaintenance)
    {
        switch (beharvior) {
        case Maintenanced:
            this->removeFirearmShowTableRow(gun);
            break;
        case RemindToMaintenance:
            if(this->updateFirearmShowTableRow(gun) == false)
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        default:
            this->updateFirearmShowTableRow(gun);
            break;
        }
    }
    else if(this->tableContentShowType == NotNeedMaintenance)
    {
        switch (beharvior) {
        case Maintenanced:
            if(this->updateFirearmShowTableRow(gun) == false)
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        case RemindToMaintenance:
            this->removeFirearmShowTableRow(gun);
            break;
        default:
            this->updateFirearmShowTableRow(gun);
            break;
        }
    }
    else
    {
        this->updateFirearmShowTableRow(gun);
    }
}

void PoliceEquipmentManager::lendCar()
{
    vehicleChooseDialog *dialog = new vehicleChooseDialog(this);
    QList<vehicle> vehicleList = this->vehicles->values();
    QList<vehicle> vehicleToLendList;

    for(int i = 0 ; i < vehicleList.length(); i++)
    {
        if(vehicleList.value(i).isInStore())
        {
            vehicleToLendList.append(vehicleList.value(i));
        }
    }
    dialog->setVehicleTable(vehicleToLendList);
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {

        policeStaffChooseDialog *staffChooseDialog = new policeStaffChooseDialog(this);
        staffChooseDialog->setPoliceOfficerTable(*(this->policeStaffs));

        staffChooseDialog->show();
        if(QDialog::Accepted == staffChooseDialog->exec())
        {
            //qDebug() << staffChooseDialog->getChoosenOne().getStaffName() << "is the driver";
            policeStaff driver = staffChooseDialog->getChoosenOne();

            vehicle carToLend = dialog->getChooseOne();
            carToLend.lendThisVehicle(QDateTime::currentDateTime(), driver);

            this->vehicles->insert(carToLend.getPlateNumber(), carToLend);

            this->DBManager->lendTheCar(carToLend);
            this->updateVehicleShowTableRow(carToLend);

        }
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::returnCar()
{
    vehicleChooseDialog *dialog = new vehicleChooseDialog(this);
    QList<vehicle> vehicleList = this->vehicles->values();
    QList<vehicle> vehicleToLendList;

    for(int i = 0 ; i < vehicleList.length(); i++)
    {
        if(vehicleList.value(i).isInStore() == false)
        {
            vehicleToLendList.append(vehicleList.value(i));
        }
    }
    dialog->setVehicleTable(vehicleToLendList);
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        vehicle carToLend = dialog->getChooseOne();
        carToLend.returnThisVehicle();

        this->vehicles->insert(carToLend.getPlateNumber(), carToLend);

        this->DBManager->returnTheCar(carToLend);

        this->updateVehicleShowTableRow(carToLend);
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::washCar()
{
    vehicleChooseDialog *dialog = new vehicleChooseDialog(this);
    QList<vehicle> vehicleList = this->vehicles->values();
    QList<vehicle> vehicleToLendList;

    for(int i = 0 ; i < vehicleList.length(); i++)
    {
        if(vehicleList.value(i).isNeedWash())
        {
            vehicleToLendList.append(vehicleList.value(i));
        }
    }
    dialog->setVehicleTable(vehicleToLendList);
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        vehicle carToLend = dialog->getChooseOne();
        carToLend.washThisVehicle(QDateTime::currentDateTime());

        this->vehicles->insert(carToLend.getPlateNumber(), carToLend);

        this->DBManager->washTheCar(carToLend);

        this->updateVehicleShowTableRow(carToLend);
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showOtherEquipmentToLend()
{
    QList<otherEquipment> equipmentList = this->equipments->values();
    QList<otherEquipment> equipmentToLendList;

    for(int i = 0 ; i < equipmentList.length(); i++)
    {
        if(equipmentList.value(i).isInStore() == true)
        {
            equipmentToLendList.append(equipmentList.value(i));
        }
    }

    otherEquipmentChooseDialog *dialog = new otherEquipmentChooseDialog(otherEquipmentChooseDialog::LendInfo, this);
    dialog->setOtherEquipmentTable(equipmentToLendList);

    if(QDialog::Accepted == dialog->exec())
    {
        this->otherEquipmentToLend = dialog->getChoosenOne();
        this->otherEquipmentToLendChoosen = true;

        this->ui->equipmentToLendLineEdit->setText(this->otherEquipmentToLend.getEquipmentId() + " " + this->otherEquipmentToLend.getEquipmentName());

    }
    dialog->close();
    delete dialog;

}

void PoliceEquipmentManager::showPoliceStaffToLendOtherEquipment()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(*(this->policeStaffs));
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->otherEquipmentLender = dialog->getChoosenOne();
        this->otherEquipmentLenderChoosen = true;

        this->ui->equipmentLenderLineEdit->setText(this->otherEquipmentLender.getPoliceNumber() + ": " + this->otherEquipmentLender.getStaffName());
    }

    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showOtherEquipmentManager()
{
    policeStaffChooseDialog *dialog = new policeStaffChooseDialog(this);
    dialog->setPoliceOfficerTable(this->getFirearmManagerList());
    dialog->show();
    if(QDialog::Accepted == dialog->exec())
    {
        this->otherEquipmentManager = dialog->getChoosenOne();
        this->otherEquipmentManagerChoosen = true;

        this->ui->equipmentManagerLineEdit->setText(this->otherEquipmentManager.getPoliceNumber() + ": " + this->otherEquipmentManager.getStaffName());
    }
    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::showOtherEquipmentToReturn()
{
    QList<otherEquipment> equipmentList = this->equipments->values();
    QList<otherEquipment> equipmentToReturnList;

    for(int i = 0 ; i < equipmentList.length(); i++)
    {
        if(equipmentList.value(i).isInStore() == false)
        {
            equipmentToReturnList.append(equipmentList.value(i));
        }
    }

    otherEquipmentChooseDialog *dialog = new otherEquipmentChooseDialog(otherEquipmentChooseDialog::ReturnInfo, this);
    dialog->setOtherEquipmentTable(equipmentToReturnList);

    if(QDialog::Accepted == dialog->exec())
    {
        this->otherEquipmentToReturn = dialog->getChoosenOne();
        this->otherEquipmentToReturnChoosen = true;

        this->ui->equipmentToReturnLineEdit->setText(this->otherEquipmentToReturn.getEquipmentId() + " " + this->otherEquipmentToReturn.getEquipmentName());

    }
    dialog->close();
    delete dialog;
}

void PoliceEquipmentManager::lendOtherEquipment()
{
    if(this->otherEquipmentToLendChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the equipment to lend"));
        return;
    }

    if(this->otherEquipmentLenderChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the police Officer who lend the equipment"));
        return;
    }

    if(this->otherEquipmentManagerChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the police Officer who manage the equipment"));
        return;
    }

    this->otherEquipmentToLend.lendThisEquipment(this->otherEquipmentLender, this->otherEquipmentManager, QDateTime::currentDateTime());
    this->equipments->insert(this->otherEquipmentToLend.getEquipmentId(), this->otherEquipmentToLend);

    this->DBManager->lendTheEquipment(this->otherEquipmentToLend);
    this->updateOtherEquipmentTableRow(this->otherEquipmentToLend);

    this->ui->equipmentToLendLineEdit->setText("");
    this->otherEquipmentToLendChoosen = false;

}

void PoliceEquipmentManager::returnOtherEquipment()
{
    if(this->otherEquipmentToReturnChoosen == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose the equipment to return"));
        return;
    }

    this->otherEquipmentToReturn.returnThisEquipment();
    this->equipments->insert(this->otherEquipmentToReturn.getEquipmentId(), this->otherEquipmentToReturn);

    this->DBManager->returnTheEquipment(this->otherEquipmentToReturn);
    this->updateOtherEquipmentTableRow(this->otherEquipmentToReturn);

    this->ui->equipmentToReturnLineEdit->setText("");
    this->otherEquipmentToReturnChoosen = false;
}

int PoliceEquipmentManager::findRowInFirearmShowTable(fireArm gun)
{
    int row = -1;
    for(int i = 0 ; i < this->ui->firearmsShowTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *firearmNumberItem = this->ui->firearmsShowTableWidget->item(i, 0);

        if(firearmNumberItem->data(Qt::UserRole).value<fireArm>() == gun)
        {
            row = i;
            break;
        }
    }
    return row;
}

void PoliceEquipmentManager::insertFirearmShowTableRow(fireArm gun)
{
    int curRow = this->ui->firearmsShowTableWidget->rowCount();
    this->ui->firearmsShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *firearmNumberItem = new QTableWidgetItem(gun.getFireArmNumber());
    QVariant bindData;
    bindData.setValue(gun);
    firearmNumberItem->setData(Qt::UserRole, bindData);

    this->ui->firearmsShowTableWidget->setItem(curRow, 0, firearmNumberItem);
    this->ui->firearmsShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(gun.getFireArmModel().getFireArmClassName()));
    this->ui->firearmsShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(gun.getFireArmLastUser().getPoliceNumber()));
    this->ui->firearmsShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(gun.getFireArmLastUser().getStaffName()));

    this->ui->firearmsShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(gun.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));
    this->ui->firearmsShowTableWidget->setItem(curRow, 5, new QTableWidgetItem(gun.getLastMaintenanceDate().toString("yyyy-MM-dd hh:mm:ss")));

    QTableWidgetItem *isNeedMaintenanceItem = new QTableWidgetItem();
    if(gun.isNeedMaintenance())
    {
        isNeedMaintenanceItem->setText(tr("Yes"));
        isNeedMaintenanceItem->setBackgroundColor(Qt::red);
    }
    else
    {
        isNeedMaintenanceItem->setText(tr("No"));
        isNeedMaintenanceItem->setBackgroundColor(Qt::green);
    }
    this->ui->firearmsShowTableWidget->setItem(curRow, 6, isNeedMaintenanceItem);

    QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
    if(gun.isInStore())
    {
        isInstoreItem->setText(tr("Yes"));
        isInstoreItem->setBackgroundColor(Qt::green);
    }
    else
    {
        isInstoreItem->setText(tr("No"));
        isInstoreItem->setBackgroundColor(Qt::red);
    }
    this->ui->firearmsShowTableWidget->setItem(curRow, 7, isInstoreItem);

}

bool PoliceEquipmentManager::updateFirearmShowTableRow(fireArm gun)
{
   int row = this->findRowInFirearmShowTable(gun);

   if(row == -1)
   {
        return false;
   }
   else
   {
       this->ui->firearmsShowTableWidget->setItem(row, 1, new QTableWidgetItem(gun.getFireArmModel().getFireArmClassName()));
       this->ui->firearmsShowTableWidget->setItem(row, 2, new QTableWidgetItem(gun.getFireArmLastUser().getPoliceNumber()));
       this->ui->firearmsShowTableWidget->setItem(row, 3, new QTableWidgetItem(gun.getFireArmLastUser().getStaffName()));

       this->ui->firearmsShowTableWidget->setItem(row, 4, new QTableWidgetItem(gun.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));
       this->ui->firearmsShowTableWidget->setItem(row, 5, new QTableWidgetItem(gun.getLastMaintenanceDate().toString("yyyy-MM-dd hh:mm:ss")));

       QTableWidgetItem *isNeedMaintenanceItem = new QTableWidgetItem();
       if(gun.isNeedMaintenance())
       {
           isNeedMaintenanceItem->setText(tr("Yes"));
           isNeedMaintenanceItem->setBackgroundColor(Qt::red);
       }
       else
       {
           isNeedMaintenanceItem->setText(tr("No"));
           isNeedMaintenanceItem->setBackgroundColor(Qt::green);
       }
       this->ui->firearmsShowTableWidget->setItem(row, 6, isNeedMaintenanceItem);

       QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
       if(gun.isInStore())
       {
           isInstoreItem->setText(tr("Yes"));
           isInstoreItem->setBackgroundColor(Qt::green);
       }
       else
       {
           isInstoreItem->setText(tr("No"));
           isInstoreItem->setBackgroundColor(Qt::red);
       }
       this->ui->firearmsShowTableWidget->setItem(row, 7, isInstoreItem);
   }

   return true;
}

void PoliceEquipmentManager::removeFirearmShowTableRow(fireArm gun)
{
    int row = this->findRowInFirearmShowTable(gun);

    if(row == -1)
    {
         return;
    }
    else
    {
        this->ui->firearmsShowTableWidget->removeRow(row);
    }
}

int PoliceEquipmentManager::findRowInVehicleShowTable(vehicle car)
{
    int row = -1;
    for(int i = 0 ; i < this->ui->vehicleShowTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *plateNumberItem = this->ui->vehicleShowTableWidget->item(i, 0);

        if(plateNumberItem->data(Qt::UserRole).value<vehicle>() == car)
        {
            row = i;
            break;
        }
    }
    return row;
}

void PoliceEquipmentManager::insertVehicleShowTableRow(vehicle car)
{
    int curRow = this->ui->vehicleShowTableWidget->rowCount();
    this->ui->vehicleShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *vehiclePlateNumberItem = new QTableWidgetItem(car.getPlateNumber());
    QVariant bindData;
    bindData.setValue(car);
    vehiclePlateNumberItem->setData(Qt::UserRole, bindData);

    this->ui->vehicleShowTableWidget->setItem(curRow, 0, vehiclePlateNumberItem);
    this->ui->vehicleShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(car.getCarType()));
    this->ui->vehicleShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(car.getLastDriver().getPoliceNumber()));
    this->ui->vehicleShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(car.getLastDriver().getStaffName()));
    this->ui->vehicleShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(car.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));
    this->ui->vehicleShowTableWidget->setItem(curRow, 5, new QTableWidgetItem(car.getLastWashDate().toString("yyyy-MM-dd hh:mm:ss")));

    QTableWidgetItem *isNeedWashItem = new QTableWidgetItem();
    if(car.isNeedWash())
    {
        isNeedWashItem->setText(tr("Yes"));
        isNeedWashItem->setBackgroundColor(Qt::red);
    }
    else
    {
        isNeedWashItem->setText(tr("No"));
        isNeedWashItem->setBackgroundColor(Qt::green);
    }
    this->ui->vehicleShowTableWidget->setItem(curRow, 6, isNeedWashItem);

    QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
    if(car.isInStore())
    {
        isInstoreItem->setText(tr("Yes"));
        isInstoreItem->setBackgroundColor(Qt::green);
    }
    else
    {
        isInstoreItem->setText(tr("No"));
        isInstoreItem->setBackgroundColor(Qt::red);
    }
    this->ui->vehicleShowTableWidget->setItem(curRow, 7, isInstoreItem);
}

bool PoliceEquipmentManager::updateVehicleShowTableRow(vehicle car)
{
    int row = this->findRowInVehicleShowTable(car);
    if(row == -1)
        return false;

    this->ui->vehicleShowTableWidget->setItem(row, 1, new QTableWidgetItem(car.getCarType()));
    this->ui->vehicleShowTableWidget->setItem(row, 2, new QTableWidgetItem(car.getLastDriver().getPoliceNumber()));
    this->ui->vehicleShowTableWidget->setItem(row, 3, new QTableWidgetItem(car.getLastDriver().getStaffName()));
    this->ui->vehicleShowTableWidget->setItem(row, 4, new QTableWidgetItem(car.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));
    this->ui->vehicleShowTableWidget->setItem(row, 5, new QTableWidgetItem(car.getLastWashDate().toString("yyyy-MM-dd hh:mm:ss")));

    QTableWidgetItem *isNeedWashItem = new QTableWidgetItem();
    if(car.isNeedWash())
    {
        isNeedWashItem->setText(tr("Yes"));
        isNeedWashItem->setBackgroundColor(Qt::red);
    }
    else
    {
        isNeedWashItem->setText(tr("No"));
        isNeedWashItem->setBackgroundColor(Qt::green);
    }
    this->ui->vehicleShowTableWidget->setItem(row, 6, isNeedWashItem);

    QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
    if(car.isInStore())
    {
        isInstoreItem->setText(tr("Yes"));
        isInstoreItem->setBackgroundColor(Qt::green);
    }
    else
    {
        isInstoreItem->setText(tr("No"));
        isInstoreItem->setBackgroundColor(Qt::red);
    }
    this->ui->vehicleShowTableWidget->setItem(row, 7, isInstoreItem);
    return true;
}

int PoliceEquipmentManager::findRowInOtherEquipmentTable(otherEquipment equipment)
{
    int row = -1;
    for(int i = 0 ; i < this->ui->otherEquipmentShowTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *equipmentIdItem = this->ui->otherEquipmentShowTableWidget->item(i, 0);

        if(equipmentIdItem->data(Qt::UserRole).value<otherEquipment>() == equipment)
        {
            row = i;
            break;
        }
    }
    return row;
}

void PoliceEquipmentManager::insertOtherEquipmentTableRow(otherEquipment equipment)
{
    int curRow = this->ui->otherEquipmentShowTableWidget->rowCount();
    this->ui->otherEquipmentShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *equipmentIdItem = new QTableWidgetItem(equipment.getEquipmentId());
    QVariant bindData;
    bindData.setValue(equipment);
    equipmentIdItem->setData(Qt::UserRole, bindData);

    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 0, equipmentIdItem);
    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(equipment.getEquipmentName()));   
    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(equipment.getEquipmentLastUser().getPoliceNumber()));
    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(equipment.getEquipmentLastUser().getStaffName()));

    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(equipment.getEquipmentLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));


    QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
    if(equipment.isInStore())
    {
        isInstoreItem->setText(tr("Yes"));
        isInstoreItem->setBackgroundColor(Qt::green);
    }
    else
    {
        isInstoreItem->setText(tr("No"));
        isInstoreItem->setBackgroundColor(Qt::red);
    }
    this->ui->otherEquipmentShowTableWidget->setItem(curRow, 5, isInstoreItem);
}

bool PoliceEquipmentManager::updateOtherEquipmentTableRow(otherEquipment equipment)
{
    int row = this->findRowInOtherEquipmentTable(equipment);
    if(row == -1)
        return false;

    this->ui->otherEquipmentShowTableWidget->setItem(row, 1, new QTableWidgetItem(equipment.getEquipmentName()));
    this->ui->otherEquipmentShowTableWidget->setItem(row, 2, new QTableWidgetItem(equipment.getEquipmentLastUser().getPoliceNumber()));
    this->ui->otherEquipmentShowTableWidget->setItem(row, 3, new QTableWidgetItem(equipment.getEquipmentLastUser().getStaffName()));

    this->ui->otherEquipmentShowTableWidget->setItem(row, 4, new QTableWidgetItem(equipment.getEquipmentLastUseDate().toString("yyyy-MM-dd hh:mm:ss")));


    QTableWidgetItem *isInstoreItem = new QTableWidgetItem();
    if(equipment.isInStore())
    {
        isInstoreItem->setText(tr("Yes"));
        isInstoreItem->setBackgroundColor(Qt::green);
    }
    else
    {
        isInstoreItem->setText(tr("No"));
        isInstoreItem->setBackgroundColor(Qt::red);
    }
    this->ui->otherEquipmentShowTableWidget->setItem(row, 5, isInstoreItem);
    return true;
}

QList<policeStaff> PoliceEquipmentManager::getFirearmManagerList()
{
    QList<policeStaff> list;

    for(int i = 0 ; i < this->policeStaffs->length(); i++)
    {
        if(this->policeStaffs->value(i).isFirearmManager())
        {
            list.append(this->policeStaffs->value(i));
        }
    }

    return list;
}

void PoliceEquipmentManager::refreshFirearrmsShowTable(PoliceEquipmentManager::firearmsShowType showType)
{
    QList<fireArm> fireArmList = this->fireArms->values();
    this->ui->firearmsShowTableWidget->clearContents();
    this->ui->firearmsShowTableWidget->setRowCount(0);

    for(int i = 0; i < fireArmList.length(); i++)
    {
        fireArm gun = fireArmList.value(i);
        switch (showType) {
        case Instore:
            if(gun.isInStore())
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        case Outstore:
            if(gun.isInStore() == false)
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        case NeedMaintenance:
            if(gun.isNeedMaintenance())
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        case NotNeedMaintenance:
            if(gun.isNeedMaintenance() == false)
            {
                this->insertFirearmShowTableRow(gun);
            }
            break;
        default:
            this->insertFirearmShowTableRow(gun);
            break;
        }
    }
    this->ui->firearmsShowTableWidget->sortByColumn(0, Qt::AscendingOrder);
}

void PoliceEquipmentManager::refreshVehicleShowTable()
{
    QList<vehicle> vehicleList = this->vehicles->values();
    this->ui->vehicleShowTableWidget->clearContents();
    this->ui->vehicleShowTableWidget->setRowCount(0);

    for(int i = 0 ; i < vehicleList.length(); i++)
    {
        this->insertVehicleShowTableRow(vehicleList.value(i));
    }
}

void PoliceEquipmentManager::refreshEquipmentShowTable()
{
    QList<otherEquipment> equipmentList = this->equipments->values();
    this->ui->otherEquipmentShowTableWidget->clearContents();;
    this->ui->otherEquipmentShowTableWidget->setRowCount(0);

    for(int i = 0 ; i < equipmentList.length(); i++)
    {
        this->insertOtherEquipmentTableRow(equipmentList.value(i));
    }
}

bool PoliceEquipmentManager::showLogin()
{
    bool loginSuccess = false;
    loginDialog *login_dialog = new loginDialog(this->managerPassword ,this);
    login_dialog->show();

    if(QDialog::Accepted == login_dialog->exec())
    {
        loginSuccess = true;
    }
    else
    {
        loginSuccess = false;
    }
    login_dialog->close();
    delete login_dialog;

    return loginSuccess;
}

