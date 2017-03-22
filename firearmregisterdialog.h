#ifndef FIREARMREGISTERDIALOG_H
#define FIREARMREGISTERDIALOG_H

#include <QDialog>
#include <QHash>
#include <QList>
#include <QTableWidgetItem>
#include "firearmclass.h"
#include "policestaff.h"
#include "firearm.h"

namespace Ui {
class fireArmRegisterDialog;
}

class fireArmRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fireArmRegisterDialog(QWidget *parent = 0);
    ~fireArmRegisterDialog();

    void setupFireArmModelTable(QList<fireArmClass> *list);
    void setupPoliceOfficerTable(QList<policeStaff> *list);

    void bindFireArmTable(QHash<QString, fireArm> *table);
signals:
    void insertFirearm(fireArm *fire_arm);
    void removeFirearm(fireArm *fire_arm);
private slots:

    void fireArmManagerChoose();
    void fireArmLastUserChoose();

    void fireArmRegister();
    void fireArmRemove();
private:
    Ui::fireArmRegisterDialog *ui;
    QHash<QString, fireArmClass> *fireArmModelTable; //枪械型号表 key为枪械类型名称
    QHash<QString, policeStaff> *policeOfficerTable; //警官列表 key为警官警号
    QHash<QString, fireArm> *fireArmTable;

    policeStaff firearmManager;
    bool firearmManagerChoosen;
    policeStaff firearmLastUser;
    bool firearmLastUserChoosen;

    void insertFirearmRow(fireArm gun);

};

#endif // FIREARMREGISTERDIALOG_H
