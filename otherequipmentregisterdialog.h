#ifndef OTHEREQUIPMENTREGISTERDIALOG_H
#define OTHEREQUIPMENTREGISTERDIALOG_H

#include <QDialog>
#include "otherequipment.h"

namespace Ui {
class otherEquipmentRegisterDialog;
}

class otherEquipmentRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit otherEquipmentRegisterDialog(QWidget *parent = 0);
    ~otherEquipmentRegisterDialog();
    void bindOtherEquipmentTable(QHash<QString, otherEquipment> *table);
signals:
    void insertOtherEquipment(otherEquipment equipment);
    void removeOtherEquipment(otherEquipment equipment);

private slots:
    void otherEquipmentRegister();
    void otherEquipmentRemove();

private:
    Ui::otherEquipmentRegisterDialog *ui;

    QHash<QString ,otherEquipment> *equipmentTable;

    void insertEquipmentRow(otherEquipment equipment);
};

#endif // OTHEREQUIPMENTREGISTERDIALOG_H
