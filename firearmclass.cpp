#include "firearmclass.h"

fireArmClass::fireArmClass()
{
    this->fireArmClassName = QObject::tr("Unkown Firearm");
    this->maintenanceDayInterval = -1;
}

fireArmClass::fireArmClass(QString fireArmClassName, int maintenanceDayInterval)
{
    this->fireArmClassName = fireArmClassName;
    this->maintenanceDayInterval = maintenanceDayInterval;
}

fireArmClass::~fireArmClass()
{

}

QString fireArmClass::getFireArmClassName()
{
    return this->fireArmClassName;
}

int fireArmClass::getMaintenanceDayInterval()
{
    return this->maintenanceDayInterval;
}

void fireArmClass::setFireArmClassName(QString fire_class_name)
{
    this->fireArmClassName = fire_class_name;
}

void fireArmClass::setMaintenanceDayInterval(int interval)
{
    this->maintenanceDayInterval = interval;
}

bool fireArmClass::operator ==(const fireArmClass &model) const
{
    if(this->fireArmClassName == model.fireArmClassName)
        return true;
    else
        return false;
}

bool fireArmClass::operator !=(const fireArmClass &model) const
{
    if(this->fireArmClassName != model.fireArmClassName)
        return true;
    else
        return false;
}
