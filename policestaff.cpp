#include "policestaff.h"

policeStaff::policeStaff()
{
    this->staffName = "";
    this->policNumber = "";
    this->isFirearmManagerStatus = false;
}

policeStaff::~policeStaff()
{

}

void policeStaff::setStaffName(QString name)
{
    this->staffName = name;
}

void policeStaff::setPoliceNumber(QString number)
{
    this->policNumber = number;
}

void policeStaff::setDepartmentName(QString name)
{
    this->departmentName = name;
}

void policeStaff::setFirearmManager(bool status)
{
    this->isFirearmManagerStatus = status;
}

QString policeStaff::getStaffName()
{
    return this->staffName;
}

QString policeStaff::getPoliceNumber()
{
    return this->policNumber;
}

QString policeStaff::getDepartmentName()
{
    return this->departmentName;
}

bool policeStaff::isFirearmManager()
{
    return this->isFirearmManagerStatus;
}

bool policeStaff::operator ==(const policeStaff &staff) const
{
    if(this->policNumber == staff.policNumber)
        return true;
    else
        return false;
}

bool policeStaff::operator !=(const policeStaff &staff) const
{
    if(this->policNumber != staff.policNumber)
        return true;
    else
        return false;
}
