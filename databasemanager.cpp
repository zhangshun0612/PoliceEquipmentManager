#include "databasemanager.h"
#include <QSqlRecord>
#include <QDebug>
DataBaseManager::DataBaseManager(QObject *parent) :
    QThread(parent)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(DBPATH);

    if(this->db.open())
    {
        qDebug() << "DataBase Open Success";
    }
    else
    {
        qDebug() << "DataBase Open Failed";
    }
}

DataBaseManager::~DataBaseManager()
{
    this->db.close();
}

void DataBaseManager::initTables()
{
    bool fireArmClassTblExist = false;
    bool policeStaffTblExist = false;
    bool firearmTblExist = false;
    bool vehicleTblExist = false;
    bool otherEquipmentTblExist = false;
    bool usagelogTblExist = false;
    bool maintenancelogTblExist = false;
    bool passwordTblExist = false;


    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.exec("select name from sqlite_master");

    while(query.next())
    {

        if(query.value(0).toString() == FIREARMCLASS_TABLE)
        {
            fireArmClassTblExist = true;
        }
        else if(query.value(0).toString() == POLICESTAFF_TABLE)
        {
            policeStaffTblExist = true;
        }
        else if(query.value(0).toString() == FIREARM_TABLE)
        {
            firearmTblExist = true;
        }
        else if(query.value(0).toString() == VEHICLE_TABLE)
        {
            vehicleTblExist = true;
        }
        else if(query.value(0).toString() == OTHER_EQUIPMENT_TABLE)
        {
            otherEquipmentTblExist = true;
        }
        else if(query.value(0).toString() == USAGELOG_TABLE)
        {
            usagelogTblExist = true;
        }
        else if(query.value(0).toString() == MAINTENANCELOG_TABLE)
        {
            maintenancelogTblExist = true;
        }
        else if(query.value(0).toString() == PASSWORD_TABLE)
        {
            passwordTblExist = true;
        }

    }

    if(fireArmClassTblExist == false)
    {
        this->createFirearmClassTable();
    }

    if(policeStaffTblExist == false)
    {
        this->createPoliceStaffTable();
    }

    if(firearmTblExist == false)
    {
        this->createFirearmTable();
    }

    if(vehicleTblExist == false)
    {
        this->createVehicleTable();
    }

    if(otherEquipmentTblExist == false)
    {
        this->createOtherEquipmentTable();
    }

    if(usagelogTblExist == false)
    {
        this->createUsagelogTable();
    }

    if(maintenancelogTblExist == false)
    {
        this->createMantenancelogTable();
    }

    if(passwordTblExist == false)
    {
        this->createPasswordTable();
        this->initPasswordTable();
    }
}

QList<fireArmClass> DataBaseManager::getFireArmClassesList()
{
    QList<fireArmClass> list;

    if(this->db.isOpen() == false)
        return list;

    QSqlQuery query(this->db);
    query.prepare("select * from " + FIREARMCLASS_TABLE);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();

            QString modelName;
            int modelMaintenanceDayInterval;
            modelName = record.value(1).toString();
            modelMaintenanceDayInterval = record.value(2).toInt();
            fireArmClass fireArmModel(modelName, modelMaintenanceDayInterval);

            list.append(fireArmModel);
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return list;
}

QList<policeStaff> DataBaseManager::getPoliceStaffsList()
{
    QList<policeStaff> list;

    if(this->db.isOpen() == false)
        return list;

    QSqlQuery query(this->db);
    query.prepare("select * from " + POLICESTAFF_TABLE);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();

            QString policeStaffNumber;
            QString policeStaffName;
            policeStaffNumber = record.value(1).toString();
            policeStaffName = record.value(2).toString();

            policeStaff staff;
            staff.setPoliceNumber(policeStaffNumber);
            staff.setStaffName(policeStaffName);
            staff.setDepartmentName(record.value(3).toString());
            staff.setFirearmManager(record.value(4).toBool());
            list.append(staff);
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return list;
}

QHash<QString, fireArm> DataBaseManager::getFireArmTable()
{
    QHash<QString, fireArm> table;

    if(this->db.isOpen() == false)
        return table;

    QSqlQuery query(this->db);
    query.prepare("select * from " + FIREARM_TABLE);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();

            QString firearmNumber = record.value(1).toString();
            QString firearmModelName = record.value(2).toString();
            QString managerPoliceNumber = record.value(3).toString();
            QString lastUserPoliceNumber = record.value(4).toString();

            int magaNumber = record.value(5).toInt();
            QDateTime lastUserDate = QDateTime::fromString(record.value(6).toString(), "yyyy-MM-dd hh:mm:ss");
            QDateTime lastMaintenanceDate = QDateTime::fromString(record.value(7).toString(), "yyyy-MM-dd hh:mm:ss");
            bool isNeedMaintenance = record.value(8).toBool();
            bool isInStore = record.value(9).toBool();

            fireArm gun;
            fireArmClass fireArmModel = getFirearmClass(firearmModelName);
            policeStaff manager = getPoliceStaff(managerPoliceNumber);
            policeStaff lastUser = getPoliceStaff(lastUserPoliceNumber);

            gun.setFireArmNumber(firearmNumber);
            gun.setFireArmModel(fireArmModel);
            gun.setFireArmManager(manager);
            gun.setFireArmLastUser(lastUser);
            gun.setMagaNumber(magaNumber);
            gun.setLastUserDate(lastUserDate);
            gun.setLastMaintenanceDate(lastMaintenanceDate);
            gun.setNeedMaintenanceStatus(isNeedMaintenance);
            gun.setInStoreStatus(isInStore);

            table.insert(firearmNumber, gun);
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }


    return table;
}

QHash<QString, vehicle> DataBaseManager::getVehicleTable()
{
    QHash<QString, vehicle> table;
    if(this->db.isOpen() == false)
        return table;

    QSqlQuery query(this->db);
    query.prepare("select * from " + VEHICLE_TABLE);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();
            vehicle car;
            car.setPlateNumber(record.value(1).toString());
            car.setCarType(record.value(2).toString());
            car.setLastDriver(getPoliceStaff(record.value(3).toString()));
            car.setLastUseDate(record.value(4).toDateTime());
            car.setLastWashDate(record.value(5).toDateTime());
            car.setNeedWashStatus(record.value(6).toBool());
            car.setInStoreStatus(record.value(7).toBool());

            table.insert(car.getPlateNumber(), car);
        }
    }
    return table;
}

QHash<QString, otherEquipment> DataBaseManager::getOtherEquipmentTable()
{
    QHash<QString, otherEquipment> table;
    if(this->db.isOpen() == false)
        return table;

    QSqlQuery query(this->db);
    query.prepare("select * from " + OTHER_EQUIPMENT_TABLE);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();
            otherEquipment equipment;

            equipment.setEquipmentId(record.value(1).toString());
            equipment.setEquipmentName(record.value(2).toString());
            equipment.setEquipmentManager(getPoliceStaff(record.value(3).toString()));
            equipment.setEquipmentLastUser(getPoliceStaff(record.value(4).toString()));
            equipment.setEquipmentLastUseDate(record.value(5).toDateTime());
            equipment.setInStoreStatus(record.value(6).toBool());

            table.insert(equipment.getEquipmentId(), equipment);
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return table;
}

void DataBaseManager::insertFirearmClassTable(fireArmClass fire_arm_class)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + FIREARMCLASS_TABLE + " values(NULL, ?, ?)");
    query.addBindValue(fire_arm_class.getFireArmClassName());
    query.addBindValue(fire_arm_class.getMaintenanceDayInterval());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::updateFirearmClassTable(fireArmClass fire_arm_class)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + FIREARMCLASS_TABLE + " set maintenance_day_interval = ? where firearm_class_name = ?");
    query.addBindValue(fire_arm_class.getMaintenanceDayInterval());
    query.addBindValue(fire_arm_class.getFireArmClassName());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::removeFirearmClassTable(fireArmClass fire_arm_class)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("delete from " + FIREARMCLASS_TABLE + " where firearm_class_name = ?");
    query.addBindValue(fire_arm_class.getFireArmClassName());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

fireArmClass DataBaseManager::getFirearmClass(QString firearm_class_name)
{
    fireArmClass firearmModel;
    if(this->db.isOpen() == false)
        return firearmModel;

    QSqlQuery query(this->db);
    query.prepare("select * from " + FIREARMCLASS_TABLE + " where firearm_class_name = ?");
    query.addBindValue(firearm_class_name);
    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();

            firearmModel.setFireArmClassName(record.value(1).toString());
            firearmModel.setMaintenanceDayInterval(record.value(2).toInt());
            break;
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }


    return firearmModel;
}

void DataBaseManager::insertPoliceStaffTable(policeStaff police_staff)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + POLICESTAFF_TABLE + " values(NULL, ?, ?, ?, ?)");
    query.addBindValue(police_staff.getPoliceNumber());
    query.addBindValue(police_staff.getStaffName());
    query.addBindValue(police_staff.getDepartmentName());
    query.addBindValue(police_staff.isFirearmManager());
    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::updatePoliceStaffNameTable(policeStaff police_staff)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + POLICESTAFF_TABLE + " set staff_name = ? where police_number = ?");
    query.addBindValue(police_staff.getStaffName());
    query.addBindValue(police_staff.getPoliceNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::updatePoliceStaffIsFirearmManager(policeStaff police_staff)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + POLICESTAFF_TABLE + " set is_firearm_manager = ? where police_number = ?");
    query.addBindValue(police_staff.isFirearmManager());
    query.addBindValue(police_staff.getPoliceNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::removePoliceStaffTable(policeStaff police_staff)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("delete from " + POLICESTAFF_TABLE + " where police_number = ?");
    query.addBindValue(police_staff.getPoliceNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}


policeStaff DataBaseManager::getPoliceStaff(QString police_number)
{
    policeStaff policeOfficer;

    if(this->db.isOpen() == false)
        return policeOfficer;

    QSqlQuery query(this->db);
    query.prepare("select * from " + POLICESTAFF_TABLE + " where police_number = ?");
    query.addBindValue(police_number);

    if(query.exec())
    {
        while(query.next())
        {
            QSqlRecord record = query.record();
            policeOfficer.setPoliceNumber(record.value(1).toString());
            policeOfficer.setStaffName(record.value(2).toString());
            policeOfficer.setDepartmentName(record.value(3).toString());
            break;
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return policeOfficer;

}

QString DataBaseManager::getPassword()
{
    QString password = "";
    if(this->db.isOpen() == false)
        return password;

    QSqlQuery query(this->db);
    QString sqlCmd = "select * from " + PASSWORD_TABLE;

    if(query.exec(sqlCmd))
    {
        while(query.next())
        {
            QSqlRecord record = query.record();
            password = record.value(1).toString();
        }
    }
    else
    {
        qDebug() << query.lastError().text();
    }

    return password;
}


void DataBaseManager::insertFirearmTable(fireArm *fire_arm)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + FIREARM_TABLE + " values(NULL, ?, ?, ?, ?, ?, ?, ?, ? ,?)");
    query.addBindValue(fire_arm->getFireArmNumber());
    query.addBindValue(fire_arm->getFireArmModel().getFireArmClassName());
    query.addBindValue(fire_arm->getFireArmManager().getPoliceNumber());
    query.addBindValue(fire_arm->getFireArmLastUser().getPoliceNumber());
    query.addBindValue(fire_arm->getMagaNumber());
    query.addBindValue(fire_arm->getLastUseDate().toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(fire_arm->getLastMaintenanceDate().toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(fire_arm->isNeedMaintenance());
    query.addBindValue(fire_arm->isInStore());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::removeFirearmTable(fireArm *fire_arm)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("delete from " + FIREARM_TABLE + " where firearm_number = ?");
    query.addBindValue(fire_arm->getFireArmNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::insertVehicleTable(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + VEHICLE_TABLE + " values(NULL, ?, ?, ?, ?, ?, ? ,?)");
    query.addBindValue(car.getPlateNumber());
    query.addBindValue(car.getCarType());
    query.addBindValue(car.getLastDriver().getPoliceNumber());
    query.addBindValue(car.getLastUseDate());
    query.addBindValue(car.getLastWashDate());
    query.addBindValue(car.isNeedWash());
    query.addBindValue(car.isInStore());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::removeVehicleTable(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("delete from " + VEHICLE_TABLE + " where plate_number = ?");
    query.addBindValue(car.getPlateNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::updateVehicleTable(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + VEHICLE_TABLE + " set car_type = ? where plate_number = ?");
    query.addBindValue(car.getCarType());
    query.addBindValue(car.getPlateNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::insertOtherEquipment(otherEquipment equipment)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + OTHER_EQUIPMENT_TABLE + " values(NULL, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(equipment.getEquipmentId());
    query.addBindValue(equipment.getEquipmentName());
    query.addBindValue(equipment.getEquipmentManager().getPoliceNumber());
    query.addBindValue(equipment.getEquipmentLastUser().getPoliceNumber());
    query.addBindValue(equipment.getEquipmentLastUseDate());
    query.addBindValue(equipment.isInStore());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::removeOtherEquipment(otherEquipment equipment)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("delete from " + OTHER_EQUIPMENT_TABLE + " where equipment_id = ?");
    query.addBindValue(equipment.getEquipmentId());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}



void DataBaseManager::lendTheFirearm(fireArm gun)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + FIREARM_TABLE + " set manager_police_number = ? ,lastuser_police_number = ? , last_use_date = ?, is_in_store = ? where firearm_number = ?");

    query.addBindValue(gun.getFireArmManager().getPoliceNumber());
    query.addBindValue(gun.getFireArmLastUser().getPoliceNumber());
    query.addBindValue(gun.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(false);
    query.addBindValue(gun.getFireArmNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

    query.prepare("insert into " + USAGELOG_TABLE + " values(NULL, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(gun.getFireArmNumber());
    query.addBindValue(gun.getFireArmModel().getFireArmClassName());
    query.addBindValue("lend");
    query.addBindValue(gun.getFireArmLastUser().getPoliceNumber());
    query.addBindValue(gun.getFireArmLastUser().getStaffName());
    query.addBindValue(gun.getFireArmManager().getPoliceNumber());
    query.addBindValue(gun.getFireArmManager().getStaffName());
    query.addBindValue(gun.getLastUseDate().toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }


}

void DataBaseManager::returnTheFirearm(fireArm gun)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + FIREARM_TABLE + " set is_in_store = ? where firearm_number = ?");
    query.addBindValue(true);
    query.addBindValue(gun.getFireArmNumber());
    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

    query.prepare("insert into " + USAGELOG_TABLE + " values(NULL, ?, ? , ?, ?, ?, ?, ?, ?)");
    query.addBindValue(gun.getFireArmNumber());
    query.addBindValue(gun.getFireArmModel().getFireArmClassName());
    query.addBindValue("return");
    query.addBindValue(gun.getFireArmLastUser().getPoliceNumber());
    query.addBindValue(gun.getFireArmLastUser().getStaffName());
    query.addBindValue(gun.getFireArmManager().getPoliceNumber());
    query.addBindValue(gun.getFireArmManager().getStaffName());
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::maintenanceTheFirearm(fireArm gun, QDateTime maintenanceDate)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + FIREARM_TABLE + " set last_maintenance_date = ? , is_need_maintenance = ? where firearm_number = ?");
    query.addBindValue(maintenanceDate.toString("yyyy-MM-dd hh:mm:ss"));
    query.addBindValue(false);
    query.addBindValue(gun.getFireArmNumber());
    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

    query.prepare("insert into " + MAINTENANCELOG_TABLE + " values(NULL, ?, ?,?)");
    query.addBindValue(gun.getFireArmNumber());
    query.addBindValue(gun.getFireArmModel().getFireArmClassName());
    query.addBindValue(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::remindMaintenanceTheFirearm(QString fireArmNumber)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + FIREARM_TABLE + " set is_need_maintenance = ? where firearm_number = ?");
    query.addBindValue(true);
    query.addBindValue(fireArmNumber);
    if(query.exec() == false)
    {
        qDebug() << query.executedQuery();
    }
}

void DataBaseManager::lendTheCar(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + VEHICLE_TABLE + " set lastuser_police_number = ?, last_use_date = ? , is_in_store = ? where plate_number = ?");\
    query.addBindValue(car.getLastDriver().getPoliceNumber());
    query.addBindValue(car.getLastUseDate());
    query.addBindValue(false);
    query.addBindValue(car.getPlateNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::returnTheCar(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + VEHICLE_TABLE + " set is_in_store = ? , is_need_wash = ? where plate_number = ?");
    query.addBindValue(true);
    query.addBindValue(true);
    query.addBindValue(car.getPlateNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::washTheCar(vehicle car)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + VEHICLE_TABLE + " set last_wash_date = ? ,is_need_wash = ? where plate_number = ?");
    query.addBindValue(car.getLastWashDate());
    query.addBindValue(false);
    query.addBindValue(car.getPlateNumber());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::lendTheEquipment(otherEquipment equipment)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + OTHER_EQUIPMENT_TABLE + " set manager_police_number = ?, lastuser_police_number = ?, last_use_date = ?, is_in_store = ? where equipment_id = ?");
    query.addBindValue(equipment.getEquipmentManager().getPoliceNumber());
    query.addBindValue(equipment.getEquipmentLastUser().getPoliceNumber());
    query.addBindValue(equipment.getEquipmentLastUseDate());
    query.addBindValue(false);
    query.addBindValue(equipment.getEquipmentId());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
    }

}

void DataBaseManager::returnTheEquipment(otherEquipment equipment)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + OTHER_EQUIPMENT_TABLE + " set is_in_store = ? where equipment_id = ?");
    query.addBindValue(true);
    query.addBindValue(equipment.getEquipmentId());

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::updatePasswordTable(QString newpassword)
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("update " + PASSWORD_TABLE + " set password = ?");
    query.addBindValue(newpassword);

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createFirearmClassTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + FIREARMCLASS_TABLE +
            "(id INTEGER primary key autoincrement, firearm_class_name TEXT, maintenance_day_interval INTEGER)";

    if(query.exec(sqlCmd))
    {
        qDebug() << FIREARMCLASS_TABLE << " Created Success";
    }
    else
    {
        qDebug() << FIREARMCLASS_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createPoliceStaffTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + POLICESTAFF_TABLE +
            "(id INTEGER primary key autoincrement, police_number TEXT, staff_name TEXT, department_name TEXT, is_firearm_manager BOOL)";

    if(query.exec(sqlCmd))
    {
        qDebug() << POLICESTAFF_TABLE << " Created Success";
    }
    else
    {
        qDebug() << POLICESTAFF_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}


void DataBaseManager::createFirearmTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + FIREARM_TABLE +
            "(id INTEGER primary key autoincrement, firearm_number TEXT, firearm_class_name TEXT, manager_police_number TEXT, lastuser_police_number TEXT, maga_number INTEGER,last_use_date TEXT , last_maintenance_date TEXT, is_need_maintenance BOOL, is_in_store BOOL)";

    if(query.exec(sqlCmd))
    {
        qDebug() << POLICESTAFF_TABLE << " Created Success";
    }
    else
    {
        qDebug() << POLICESTAFF_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createVehicleTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + VEHICLE_TABLE +
            "(id INTEGER primary key autoincrement, plate_number TEXT, car_type TEXT, lastuser_police_number TEXT, last_use_date TEXT, last_wash_date TEXT, is_need_wash BOOL, is_in_store BOOL)";

    if(query.exec(sqlCmd))
    {
        qDebug() << VEHICLE_TABLE << " Created Success";
    }
    else
    {
        qDebug() << VEHICLE_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createOtherEquipmentTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + OTHER_EQUIPMENT_TABLE +
            "(id INTEGER primary key autoincrement, equipment_id TEXT, equipment_name TEXT, manager_police_number TEXT, lastuser_police_number TEXT, last_use_date TEXT,  is_in_store BOOL)";

    if(query.exec(sqlCmd))
    {
        qDebug() << OTHER_EQUIPMENT_TABLE << " Created Success";
    }
    else
    {
        qDebug() << OTHER_EQUIPMENT_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createUsagelogTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + USAGELOG_TABLE +
             "(id INTEGER primary key autoincrement, firearm_number TEXT, firearm_model TEXT, behavior TEXT ,user_police_number TEXT, user_police_name TEXT, manager_police_number TEXT, manager_police_name TEXT,record_date TEXT)";
    if(query.exec(sqlCmd))
    {
        qDebug() << USAGELOG_TABLE << " Created Success";
    }
    else
    {
        qDebug() << USAGELOG_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::createMantenancelogTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + MAINTENANCELOG_TABLE +
             "(id INTEGER primary key autoincrement, firearm_number TEXT, firearm_model TEXT, record_date TEXT)";
    if(query.exec(sqlCmd))
    {
        qDebug() << MAINTENANCELOG_TABLE << " Created Success";
    }
    else
    {
        qDebug() << MAINTENANCELOG_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }
}

void DataBaseManager::createPasswordTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    QString sqlCmd = "create table " + PASSWORD_TABLE + "(id INTEGER primary key autoincrement, password TEXT)";

    if(query.exec(sqlCmd))
    {
        qDebug() << PASSWORD_TABLE << " Created Success";
    }
    else
    {
        qDebug() << PASSWORD_TABLE << " Created Failed";
        qDebug() << query.lastError().text();
    }

}

void DataBaseManager::initPasswordTable()
{
    if(this->db.isOpen() == false)
        return;

    QSqlQuery query(this->db);
    query.prepare("insert into " + PASSWORD_TABLE + " values(NULL, ?)");
    query.addBindValue("123456");

    if(query.exec() == false)
    {
        qDebug() << query.lastError().text();
    }
}


