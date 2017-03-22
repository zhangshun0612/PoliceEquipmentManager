#ifndef VEHICLE_H
#define VEHICLE_H

#include <QString>
#include <QDateTime>

#include "policestaff.h"
class vehicle
{
public:
    vehicle();
    ~vehicle();

    void setPlateNumber(QString number);
    void setCarType(QString type);
    void setLastDriver(policeStaff driver);
    void setNeedWashStatus(bool status);
    void setInStoreStatus(bool status);
    void setLastUseDate(QDateTime date);
    void setLastWashDate(QDateTime date);

    void lendThisVehicle(QDateTime dateToLend, policeStaff driver);
    void returnThisVehicle();
    void washThisVehicle(QDateTime dateToWash);

    QString getPlateNumber();
    QString getCarType();
    policeStaff getLastDriver();
    bool isNeedWash();
    bool isInStore();
    QDateTime getLastUseDate();
    QDateTime getLastWashDate();

    bool operator ==(const vehicle &car) const;
    bool operator !=(const vehicle &car) const;

private:
    QString plateNumber; //车牌号
    QString carType; //车型
    policeStaff lastDriver;
    bool isNeedWashStatus; //车辆是否需要清洗
    bool isInStoreStatus; //车辆是否在车库中
    QDateTime lastUseDate; //上次使用时间
    QDateTime lastWashDate; //上次清洗时间

};

#endif // VEHICLE_H
