#ifndef POLICESTAFFCHOOSEDIALOG_H
#define POLICESTAFFCHOOSEDIALOG_H

#include <QDialog>
#include <QHash>
#include <QTableWidgetItem>

#include "policestaff.h"

namespace Ui {
class policeStaffChooseDialog;
}

class policeStaffChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit policeStaffChooseDialog(QWidget *parent = 0);
    ~policeStaffChooseDialog();

    void setPoliceOfficerTable(QList<policeStaff> list) ;

    policeStaff getChoosenOne();
private slots:
    void choosePushButtonClicked();
private:
    Ui::policeStaffChooseDialog *ui;
    policeStaff choosenOne;
};

#endif // POLICESTAFFCHOOSEDIALOG_H
