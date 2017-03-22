#ifndef POLICEEQUIPMENTMANAGER_H
#define POLICEEQUIPMENTMANAGER_H

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QTimer>
#include "databasemanager.h"
#include "firearmclass.h"
#include "policestaff.h"
#include "firearm.h"
#include "vehicle.h"

namespace Ui {
class PoliceEquipmentManager;
}

class PoliceEquipmentManager : public QMainWindow
{
    Q_OBJECT

public:
    enum firearmsShowType{
        Instore, //库存枪支
        Outstore, //出库枪支
        NeedMaintenance, //需保养枪支
        NotNeedMaintenance, //不需要养枪支
        All //所有枪支
    };

    //导致枪支状态改变的行为
    enum firearmStatusChangedBehavior{
        LendOut, //接出
        ReturnBack, //归还
        Maintenanced, //保养完毕
        RemindToMaintenance //提醒需要保养
    };

    explicit PoliceEquipmentManager(QWidget *parent = 0);
    ~PoliceEquipmentManager();
public slots:
    void showFirearmMainWidget();
    void showVehicleMainWidget();
    void showOtherEquipmentMainWidget();

    void showFirearmClassRegisterDialog();
    void showPoliceStaffRegisterDialog();
    void showPoliceManagerRegisterDialog();
    void showFireArmRegisterDialog();
    void showVehicleRegisterDialog();
    void showOtherEquipmentRegisterDialog();
    void showInquiryRecordDialog();

    void showPasswordChangeDialog();

    void showFirearmToLend();
    void showPoliceStaffToLendGun();
    void showFirearmManager();
    void lendGun();

    void showFirearmToReturn();
    void returnGun();

    void showFirearmToMaintenance();
    void maintenanceGun();

    void checkFirearmMaintenaceStatus();

    void refreshFireArmsTable();
    void refreshVehicleTable();
    void refreshOtherEquipmentTable();

    void fireArmShowTableContentsToShowChanged();
    void fireArmStatusChanged(fireArm gun, enum firearmStatusChangedBehavior beharvior);

    void lendCar();
    void returnCar();
    void washCar();

    void showOtherEquipmentToLend();
    void showPoliceStaffToLendOtherEquipment();
    void showOtherEquipmentManager();
    void showOtherEquipmentToReturn();

    void lendOtherEquipment();
    void returnOtherEquipment();

private:
    Ui::PoliceEquipmentManager *ui;
    QList<fireArmClass> *fireArmClasses;
    QList<policeStaff> *policeStaffs;
    QHash<QString , fireArm> *fireArms;
    QHash<QString, vehicle> *vehicles;
    QHash<QString, otherEquipment> *equipments;
    DataBaseManager *DBManager;
    QTimer *firearmMaintenanceCheckTimer;

    bool firearmToLendChoosen;
    fireArm firearmToLend;
    bool firearmLenderChoosen;
    policeStaff firearmLender;
    bool firearmManagerChoosen;
    policeStaff firearmManager;
    bool firearmToReturnChoosen;
    fireArm firearmToReturn;
    bool firearmToMaintenanceChoosen;
    fireArm firearmToMaintenance;

    bool otherEquipmentLenderChoosen;
    policeStaff otherEquipmentLender;
    bool otherEquipmentManagerChoosen;
    policeStaff otherEquipmentManager;
    bool otherEquipmentToLendChoosen;
    otherEquipment otherEquipmentToLend;
    bool otherEquipmentToReturnChoosen;
    otherEquipment otherEquipmentToReturn;

    enum firearmsShowType tableContentShowType;

    QString managerPassword;

    int findRowInFirearmShowTable(fireArm gun);
    void insertFirearmShowTableRow(fireArm gun);
    bool updateFirearmShowTableRow(fireArm gun);
    void removeFirearmShowTableRow(fireArm gun);

    int findRowInVehicleShowTable(vehicle car);
    void insertVehicleShowTableRow(vehicle car);
    bool updateVehicleShowTableRow(vehicle car);

    int findRowInOtherEquipmentTable(otherEquipment equipment);
    void insertOtherEquipmentTableRow(otherEquipment equipment);
    bool updateOtherEquipmentTableRow(otherEquipment equipment);

    QList<policeStaff> getFirearmManagerList();
    void refreshFirearrmsShowTable(enum firearmsShowType showType);
    void refreshVehicleShowTable();
    void refreshEquipmentShowTable();

    bool showLogin();
};

#endif // POLICEEQUIPMENTMANAGER_H
