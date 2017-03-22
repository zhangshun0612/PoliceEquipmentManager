#include "vehicle.h"

vehicle::vehicle()
{
    this->isNeedWashStatus = false;
    this->isInStoreStatus = true;
}

vehicle::~vehicle()
{

}

void vehicle::setPlateNumber(QString number)
{
    this->plateNumber = number;
}

void vehicle::setCarType(QString type)
{
    this->carType = type;
}

void vehicle::setLastDriver(policeStaff driver)
{
    this->lastDriver = driver;
}

void vehicle::setNeedWashStatus(bool status)
{
    this->isNeedWashStatus = status;
}

void vehicle::setInStoreStatus(bool status)
{
    this->isInStoreStatus = status;
}

void vehicle::setLastUseDate(QDateTime date)
{
    this->lastUseDate = date;
}

void vehicle::setLastWashDate(QDateTime date)
{
    this->lastWashDate = date;
}

void vehicle::lendThisVehicle(QDateTime dateToLend, policeStaff driver)
{
    this->lastUseDate = dateToLend;
    this->lastDriver = driver;
    this->isInStoreStatus = false;
}

void vehicle::returnThisVehicle()
{
    this->isInStoreStatus = true;
    this->isNeedWashStatus = true;
}

void vehicle::washThisVehicle(QDateTime dateToWash)
{
    this->lastWashDate = dateToWash;
    this->isNeedWashStatus = false;
}

QString vehicle::getPlateNumber()
{
    return this->plateNumber;
}

QString vehicle::getCarType()
{
    return this->carType;
}

policeStaff vehicle::getLastDriver()
{
    return this->lastDriver;
}

bool vehicle::isNeedWash()
{
    return this->isNeedWashStatus;
}

bool vehicle::isInStore()
{
    return this->isInStoreStatus;
}

QDateTime vehicle::getLastUseDate()
{
    return this->lastUseDate;
}

QDateTime vehicle::getLastWashDate()
{
    return this->lastWashDate;
}

bool vehicle::operator ==(const vehicle &car) const
{
    if(this->plateNumber == car.plateNumber)
        return true;
    else
        return false;
}

bool vehicle::operator !=(const vehicle &car) const
{
    if(this->plateNumber != car.plateNumber)
        return true;
    else
        return false;
}





