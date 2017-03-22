#ifndef FIREARMCLASS_H
#define FIREARMCLASS_H

#include <QObject>
#include <QString>

//枪支类型 类
class fireArmClass
{
public:
    fireArmClass();
    fireArmClass(QString fireArmClassName, int maintenanceDayInterval);
    ~fireArmClass();

    QString getFireArmClassName();
    int getMaintenanceDayInterval();

    void setFireArmClassName(QString fire_class_name);
    void setMaintenanceDayInterval(int interval);

    bool operator ==(const fireArmClass &model) const;
    bool operator !=(const fireArmClass &model) const;
private:
    QString fireArmClassName; //枪支类型名称
    int maintenanceDayInterval; //保养天数周期

};

#endif // FIREARMCLASS_H
