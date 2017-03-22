#ifndef FIREARM_H
#define FIREARM_H

#include <QDateTime>
#include "firearmclass.h"
#include "policestaff.h"
class fireArm
{
public:
    fireArm();
    ~fireArm();

    void setFireArmNumber(QString fire_arm_number);
    void setFireArmModel(fireArmClass model);
    void setFireArmLastUser(policeStaff staff);
    void setFireArmManager(policeStaff staff);
    void setMagaNumber(int number);
    void setLastUserDate(QDateTime date);
    void setLastMaintenanceDate(QDateTime date);
    void setNeedMaintenanceStatus(bool status);
    void setInStoreStatus(bool status);

    void lendThisFirearm(policeStaff lender, policeStaff manager, QDateTime dateToLend);
    void returnThisFirearm();
    void maintenanceThisFirearm(QDateTime dateToMaintenance);

    QString getFireArmNumber();
    fireArmClass getFireArmModel();
    policeStaff getFireArmLastUser();
    policeStaff getFireArmManager();
    int getMagaNumber();
    QDateTime getLastUseDate();
    QDateTime getLastMaintenanceDate();
    bool isNeedMaintenance();
    bool isInStore();

    bool operator ==(const fireArm &gun) const;
    bool operator !=(const fireArm &gun) const;
signals:

public slots:

private:
    QString fireArmNumber;
    fireArmClass fireArmModel; //枪支型号
    policeStaff lastUser; //上次使用者
    policeStaff manager;   //枪支管理者

    int magaNumber; //弹夹数

    QDateTime lastUseDate; //上次使用时间
    QDateTime lastMaintenanceDate; //上次保养时间
    bool isNeedMaintenanceStatus; //是否需要保养
    bool isInStoreStatus; //是否在仓库中
};

#endif // FIREARM_H
