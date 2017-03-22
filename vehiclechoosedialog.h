#ifndef VEHICLECHOOSEDIALOG_H
#define VEHICLECHOOSEDIALOG_H

#include <QDialog>

#include "vehicle.h"

namespace Ui {
class vehicleChooseDialog;
}

class vehicleChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vehicleChooseDialog(QWidget *parent = 0);
    ~vehicleChooseDialog();

    void setVehicleTable(QList<vehicle> list);
    vehicle getChooseOne();

private slots:
    void choosePushButtonClicked();
private:
    Ui::vehicleChooseDialog *ui;
    vehicle chooseOne;
};

#endif // VEHICLECHOOSEDIALOG_H
