#ifndef FIREARMCHOOSEDIALOG_H
#define FIREARMCHOOSEDIALOG_H

#include <QDialog>

#include "firearm.h"
#include "firearmclass.h"
#include "policestaff.h"
namespace Ui {
class fireArmChooseDialog;
}

class fireArmChooseDialog : public QDialog
{
    Q_OBJECT

public:
    enum fireArmChooseDialogType{
        LendInfo,
        ReturnInfo,
        MaintenanceInfo
    };

    explicit fireArmChooseDialog(enum fireArmChooseDialogType type, QWidget *parent = 0);
    ~fireArmChooseDialog();

    void setFirearmTable(QList<fireArm> list);

    fireArm getChoosenOne();
private slots:
    void choosePushButtonClicked();
    void searchPushButtonClicked();
private:
    Ui::fireArmChooseDialog *ui;
    enum fireArmChooseDialogType dialogType;

    fireArm choosenOne;
};

#endif // FIREARMCHOOSEDIALOG_H
