#ifndef FIREARMMANAGERREGISTERDIALOG_H
#define FIREARMMANAGERREGISTERDIALOG_H

#include <QDialog>
#include "policestaff.h"
#include "policestaffchoosedialog.h"
namespace Ui {
class firearmManagerRegisterDialog;
}

class firearmManagerRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit firearmManagerRegisterDialog(QWidget *parent = 0);
    ~firearmManagerRegisterDialog();

    void setPoliceStaffList(QList<policeStaff> *list);
signals:
    void updatePoliceStaffisManagerStatus(policeStaff police_staff);
private slots:
    void showPoliceStaffCanBeManager();
    void removeFirearmManager();
private:
    Ui::firearmManagerRegisterDialog *ui;

    QList<policeStaff> *policeStaffList;
    QList<policeStaff> managerList;
};

#endif // FIREARMMANAGERREGISTERDIALOG_H
