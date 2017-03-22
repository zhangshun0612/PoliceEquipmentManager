#ifndef OTHEREQUIPMENT_H
#define OTHEREQUIPMENT_H
#include <QString>
#include <QDateTime>

#include "policestaff.h"
class otherEquipment
{
public:
    otherEquipment();
    ~otherEquipment();

    void setEquipmentId(QString id);
    void setEquipmentName(QString name);
    void setEquipmentLastUser(policeStaff staff);
    void setEquipmentLastUseDate(QDateTime date);
    void setEquipmentManager(policeStaff staff);
    void setInStoreStatus(bool status);

    void lendThisEquipment(policeStaff staff, policeStaff manager, QDateTime dateToLend);
    void returnThisEquipment();

    QString getEquipmentId();
    QString getEquipmentName();
    policeStaff getEquipmentLastUser();
    QDateTime getEquipmentLastUseDate();
    policeStaff getEquipmentManager();

    bool isInStore();



    bool operator ==(const otherEquipment &equipment) const;
    bool operator !=(const otherEquipment &equipment) const;

private:
    QString equipmentId;
    QString equipmentName;

    QDateTime lastUseDate;
    policeStaff lastUser; //上次使用者
    policeStaff manager;

    bool isInStoreStatus;


};

#endif // OTHEREQUIPMENT_H
