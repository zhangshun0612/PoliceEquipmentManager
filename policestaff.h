#ifndef POLICESTAFF_H
#define POLICESTAFF_H

#include <QString>
#include <QObject>
//警务工作人员类
class policeStaff
{
public:
    policeStaff();
    ~policeStaff();

    void setStaffName(QString name);
    void setPoliceNumber(QString number);
    void setDepartmentName(QString name);
    void setFirearmManager(bool status);

    QString getStaffName();
    QString getPoliceNumber();
    QString getDepartmentName();
    bool isFirearmManager();


    bool operator ==(const policeStaff &staff) const;
    bool operator !=(const policeStaff &staff) const;
private:
    QString staffName; //人员名称
    QString policNumber; //警号
    QString departmentName; //部门编号
    bool isFirearmManagerStatus;
};

#endif // POLICESTAFF_H
