#ifndef POLICESTAFFREGESTERDIALOG_H
#define POLICESTAFFREGESTERDIALOG_H

#include <QDialog>
#include <QList>
#include <QTableWidgetItem>
#include "policestaff.h"
namespace Ui {
class policeStaffRegesterDialog;
}

class policeStaffRegesterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit policeStaffRegesterDialog(QWidget *parent = 0);
    ~policeStaffRegesterDialog();

    void setPoliceStaffList(QList<policeStaff> *list);
private slots:
    void insertPushButtonClicked();
    void removePushButtonClicked();

    void policeStaffNameItemEditting(QTableWidgetItem *item);
    void policeStaffNameItemEditted(QTableWidgetItem *item);
signals:
    void insertPoliceStaff(policeStaff police_staff);
    void updatePoliceStaff(policeStaff police_staff);
    void removePoliceStaff(policeStaff police_staff);
private:
    Ui::policeStaffRegesterDialog *ui;
    QList<policeStaff> *policeStaffList;

    QTableWidgetItem *edittingItem;
    QString valueBeforeEditting;
};

#endif // POLICESTAFFREGESTERDIALOG_H
