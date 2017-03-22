#ifndef VEHICLEREGISTERDIALOG_H
#define VEHICLEREGISTERDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "vehicle.h"
namespace Ui {
class vehicleRegisterDialog;
}

class vehicleRegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vehicleRegisterDialog(QWidget *parent = 0);
    ~vehicleRegisterDialog();
    void bindVehicleTable(QHash<QString, vehicle> *table);
signals:
    void insertVehicle(vehicle car);
    void removeVehicle(vehicle car);
    void updateVehicle(vehicle car);
private slots:
    void vehicleRigister();
    void vehicleRemove();

    void vehicleTypeEditting(QTableWidgetItem *item);
    void vehicleTypeItemEdited(QTableWidgetItem *item);
private:
    Ui::vehicleRegisterDialog *ui;
    QHash<QString, vehicle> *vehicleTable;
    QTableWidgetItem *edittingItem;
    QString valueBeforeEditting;

    void insertVehicleRow(vehicle car);
    QString transferPlateNumber(QString str);
};

#endif // VEHICLEREGISTERDIALOG_H
