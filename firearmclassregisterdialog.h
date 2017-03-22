#ifndef FIREARMCLASSREGISTERDIALOG_H
#define FIREARMCLASSREGISTERDIALOG_H

#include <QDialog>
#include <QList>
#include <QTableWidgetItem>
#include "firearmclass.h"

namespace Ui {
class fireArmClassRegisterDialog;
}

class fireArmClassRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fireArmClassRegisterDialog(QWidget *parent = 0);
    ~fireArmClassRegisterDialog();

    void setFireArmClassList(QList<fireArmClass> *list);

private slots:
    void insertPushButtonClicked();
    void removePushButtonClicked();

    void firearmMaintenanceDayIntervalItemEditting(QTableWidgetItem *item);
    void firearmMaintenanceDayIntervalItemEdited(QTableWidgetItem *item);
signals:
    void insertFireArmClass(fireArmClass fire_arm_class);
    void updateFireArmClass(fireArmClass fire_arm_class);
    void removeFireArmClass(fireArmClass fire_arm_class);
private:
    Ui::fireArmClassRegisterDialog *ui;
    QList<fireArmClass> *fireArmClassList;
    QTableWidgetItem *edittingItem;
    QString valueBeforeEditting;
};

#endif // FIREARMCLASSREGISTERDIALOG_H
