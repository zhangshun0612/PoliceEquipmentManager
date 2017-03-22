#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QList>
#include <QHash>

#include "firearmclass.h"
#include "policestaff.h"
#include "firearm.h"
#include "vehicle.h"
#include "otherequipment.h"

#define DBPATH "./INFODB.db"
#define FIREARMCLASS_TABLE QString("firearmclass_table")
#define POLICESTAFF_TABLE QString("policestaff_table")
#define FIREARM_TABLE QString("firearm_table")
#define VEHICLE_TABLE QString("vehicle_table")
#define OTHER_EQUIPMENT_TABLE QString("other_equipment_table")
#define USAGELOG_TABLE QString("usagelog_table")
#define MAINTENANCELOG_TABLE QString("maintenancelog_table")
#define PASSWORD_TABLE QString("password_table")

class DataBaseManager : public QThread
{
    Q_OBJECT
public:
    explicit DataBaseManager(QObject *parent = 0);
    ~DataBaseManager();

    void initTables();
    QList<fireArmClass> getFireArmClassesList();
    QList<policeStaff> getPoliceStaffsList();
    QHash<QString, fireArm> getFireArmTable();
    QHash<QString, vehicle> getVehicleTable();
    QHash<QString, otherEquipment> getOtherEquipmentTable();

    fireArmClass getFirearmClass(QString firearm_class_name);
    policeStaff getPoliceStaff(QString police_number);

    QString getPassword();
signals:

public slots:
    void insertFirearmClassTable(fireArmClass fire_arm_class);
    void updateFirearmClassTable(fireArmClass fire_arm_class);
    void removeFirearmClassTable(fireArmClass fire_arm_class);


    void insertPoliceStaffTable(policeStaff police_staff);
    void updatePoliceStaffNameTable(policeStaff police_staff);
    void updatePoliceStaffIsFirearmManager(policeStaff police_staff);
    void removePoliceStaffTable(policeStaff police_staff);

    void insertFirearmTable(fireArm *fire_arm);
    void removeFirearmTable(fireArm *fire_arm);

    void insertVehicleTable(vehicle car);
    void removeVehicleTable(vehicle car);
    void updateVehicleTable(vehicle car);

    void insertOtherEquipment(otherEquipment equipment);
    void removeOtherEquipment(otherEquipment equipment);


    void lendTheFirearm(fireArm gun);
    void returnTheFirearm(fireArm gun);
    void maintenanceTheFirearm(fireArm gun, QDateTime maintenanceDate);
    void remindMaintenanceTheFirearm(QString fireArmNumber);

    void lendTheCar(vehicle car);
    void returnTheCar(vehicle car);
    void washTheCar(vehicle car);

    void lendTheEquipment(otherEquipment equipment);
    void returnTheEquipment(otherEquipment equipment);


    void updatePasswordTable(QString newpassword);
private:
    QSqlDatabase db;
    void createFirearmClassTable();
    void createPoliceStaffTable();
    void createFirearmTable();
    void createVehicleTable();
    void createOtherEquipmentTable();
    void createUsagelogTable();
    void createMantenancelogTable();
    void createPasswordTable();
    void initPasswordTable();
};

#endif // DATABASEMANAGER_H
