#ifndef OTHEREQUIPMENTCHOOSEDIALOG_H
#define OTHEREQUIPMENTCHOOSEDIALOG_H

#include <QDialog>

#include "otherequipment.h"
namespace Ui {
class otherEquipmentChooseDialog;
}

class otherEquipmentChooseDialog : public QDialog
{
    Q_OBJECT

public:
    enum otherEquipmentDialogType{
        LendInfo,
        ReturnInfo,
        MaintenanceInfo
    };

    explicit otherEquipmentChooseDialog(enum otherEquipmentDialogType type, QWidget *parent = 0);
    ~otherEquipmentChooseDialog();

    void setOtherEquipmentTable(QList<otherEquipment> list);
    otherEquipment getChoosenOne();
private slots:
    void choosePushButtonClicked();
private:
    Ui::otherEquipmentChooseDialog *ui;
    enum otherEquipmentDialogType dialogType;
    otherEquipment choosenOne;
};

#endif // OTHEREQUIPMENTCHOOSEDIALOG_H
