#include "otherequipment.h"

otherEquipment::otherEquipment()
{
    this->equipmentName = "Unknown Equipment";

    this->isInStoreStatus = true;
}

otherEquipment::~otherEquipment()
{

}

void otherEquipment::setEquipmentId(QString id)
{
    this->equipmentId = id;
}

void otherEquipment::setEquipmentName(QString name)
{
    this->equipmentName = name;
}

void otherEquipment::setEquipmentLastUser(policeStaff staff)
{
    this->lastUser = staff;
}

void otherEquipment::setEquipmentLastUseDate(QDateTime date)
{
    this->lastUseDate = date;
}

void otherEquipment::setEquipmentManager(policeStaff staff)
{
    this->manager = staff;
}

void otherEquipment::setInStoreStatus(bool status)
{
    this->isInStoreStatus = status;
}

void otherEquipment::lendThisEquipment(policeStaff staff, policeStaff manager, QDateTime dateToLend)
{
    this->lastUser = staff;
    this->manager = manager;
    this->lastUseDate = dateToLend;
    this->isInStoreStatus = false;
}

void otherEquipment::returnThisEquipment()
{
    this->isInStoreStatus = true;
}

QString otherEquipment::getEquipmentId()
{
    return this->equipmentId;
}

QString otherEquipment::getEquipmentName()
{
    return this->equipmentName;
}

policeStaff otherEquipment::getEquipmentLastUser()
{
    return this->lastUser;
}

QDateTime otherEquipment::getEquipmentLastUseDate()
{
    return this->lastUseDate;
}

policeStaff otherEquipment::getEquipmentManager()
{
    return this->manager;
}

bool otherEquipment::isInStore()
{
    return this->isInStoreStatus;
}

bool otherEquipment::operator ==(const otherEquipment &equipment) const
{
    if(this->equipmentId == equipment.equipmentId)
        return true;
    else
        return false;
}

bool otherEquipment::operator !=(const otherEquipment &equipment) const
{
    if(this->equipmentId != equipment.equipmentId)
        return true;
    else
        return false;
}



