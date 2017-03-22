#include "firearm.h"

fireArm::fireArm()
{
    this->fireArmNumber = "0000000";
    this->magaNumber = 2;

    this->isInStoreStatus = true;
    this->isNeedMaintenanceStatus = false;
}

fireArm::~fireArm()
{

}

void fireArm::setFireArmNumber(QString fire_arm_number)
{
    this->fireArmNumber = fire_arm_number.trimmed().replace(" ", "");
}

void fireArm::setFireArmModel(fireArmClass model)
{
    this->fireArmModel = model;
}

void fireArm::setFireArmLastUser(policeStaff staff)
{
    this->lastUser = staff;
}

void fireArm::setFireArmManager(policeStaff staff)
{
    this->manager = staff;
}

void fireArm::setMagaNumber(int number)
{
    this->magaNumber = number;
}

void fireArm::setLastUserDate(QDateTime date)
{
    this->lastUseDate = date;
}

void fireArm::setLastMaintenanceDate(QDateTime date)
{
    this->lastMaintenanceDate = date;
}

void fireArm::setNeedMaintenanceStatus(bool status)
{
    this->isNeedMaintenanceStatus = status;
}

void fireArm::setInStoreStatus(bool status)
{
    this->isInStoreStatus = status;
}

void fireArm::lendThisFirearm(policeStaff lender, policeStaff manager, QDateTime dateToLend)
{
    this->lastUser = lender;
    this->manager = manager;
    this->lastUseDate = dateToLend;
    this->isInStoreStatus = false;
}

void fireArm::returnThisFirearm()
{
    this->isInStoreStatus = true;
}

void fireArm::maintenanceThisFirearm(QDateTime dateToMaintenance)
{
    this->lastMaintenanceDate = dateToMaintenance;
    this->isNeedMaintenanceStatus = false;
}



QString fireArm::getFireArmNumber()
{
    return this->fireArmNumber;
}

fireArmClass fireArm::getFireArmModel()
{
    return this->fireArmModel;
}

policeStaff fireArm::getFireArmLastUser()
{
    return this->lastUser;
}

policeStaff fireArm::getFireArmManager()
{
    return this->manager;
}

int fireArm::getMagaNumber()
{
    return this->magaNumber;
}

QDateTime fireArm::getLastUseDate()
{
    return this->lastUseDate;
}

QDateTime fireArm::getLastMaintenanceDate()
{
    return this->lastMaintenanceDate;
}

bool fireArm::isNeedMaintenance()
{
    return this->isNeedMaintenanceStatus;
}

bool fireArm::isInStore()
{
    return this->isInStoreStatus;
}

bool fireArm::operator ==(const fireArm &gun) const
{
    if(this->fireArmNumber == gun.fireArmNumber)
        return true;
    else
        return false;
}

bool fireArm::operator !=(const fireArm &gun) const
{
    if(this->fireArmNumber != gun.fireArmNumber)
        return true;
    else
        return false;
}

