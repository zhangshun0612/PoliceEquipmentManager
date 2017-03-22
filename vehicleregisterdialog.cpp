#include "vehicleregisterdialog.h"
#include "ui_vehicleregisterdialog.h"

#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(vehicle)
vehicleRegisterDialog::vehicleRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vehicleRegisterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->vehicleShowTableWidget->setColumnCount(2);
    this->ui->vehicleShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Plate Number") << tr("Vehicle Type"));
    this->ui->vehicleShowTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(this->ui->insertPushButton, SIGNAL(clicked()), this, SLOT(vehicleRigister()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(vehicleRemove()));
    connect(this->ui->vehicleShowTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(vehicleTypeEditting(QTableWidgetItem*)));
    connect(this->ui->vehicleShowTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(vehicleTypeItemEdited(QTableWidgetItem*)));
}

vehicleRegisterDialog::~vehicleRegisterDialog()
{
    delete ui;
}

void vehicleRegisterDialog::bindVehicleTable(QHash<QString, vehicle> *table)
{
    this->vehicleTable = table;

    QList<vehicle> carlist = this->vehicleTable->values();
    for(int i = 0 ; i < carlist.length(); i++)
    {
        this->insertVehicleRow(carlist.value(i));
    }

}


void vehicleRegisterDialog::vehicleRigister()
{
    QRegExp rx("^[A-Z]{1}[A-Z_0-9]{5}$");
    QString letterAndNumber = this->ui->inputPlateNumberLineEdit->text().trimmed().toUpper().replace(" ", "");

    if(letterAndNumber.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Plate Number"));
        return;
    }

    /*
    if(rx.exactMatch(letterAndNumber) == false)
    {
        QMessageBox::warning(this, tr("Error"), tr("Invalid Plate Number"));
        return;
    }
    */

    QString plateNumber = this->ui->provinceShortNameComboBox->currentText() + transferPlateNumber(letterAndNumber);
    this->ui->inputPlateNumberLineEdit->setText(transferPlateNumber(letterAndNumber));
    QString carType = this->ui->inputVehicleTypeLineEdit->text();
    if(carType.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Car Type"));
        return;
    }

    if(this->vehicleTable->contains(plateNumber))
    {
        QMessageBox::warning(this, tr("Error"), tr("Plate Number has contained"));
        return;
    }

    vehicle car;
    car.setPlateNumber(plateNumber);
    car.setCarType(carType);

    this->vehicleTable->insert(plateNumber, car);
    this->insertVehicleRow(car);

    emit this->insertVehicle(car);
}

void vehicleRegisterDialog::vehicleRemove()
{
    int curRow = this->ui->vehicleShowTableWidget->currentRow();
    if(curRow == -1)
        return;

    QTableWidgetItem *plateNumberItem = this->ui->vehicleShowTableWidget->item(curRow, 0);
    vehicle car = plateNumberItem->data(Qt::UserRole).value<vehicle>();

    this->vehicleTable->remove(car.getPlateNumber());
    this->ui->vehicleShowTableWidget->removeRow(curRow);
    emit this->removeVehicle(car);
}

void vehicleRegisterDialog::vehicleTypeEditting(QTableWidgetItem *item)
{
    this->valueBeforeEditting = item->text();
    this->edittingItem = item;
}

void vehicleRegisterDialog::vehicleTypeItemEdited(QTableWidgetItem *item)
{
    if(this->edittingItem != item)
        return;

    QString carType = item->text();
    if(carType.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Car Type"));
        item->setText(this->valueBeforeEditting);
        return;
    }

    int row = this->ui->vehicleShowTableWidget->row(item);
    vehicle car = this->ui->vehicleShowTableWidget->item(row, 0)->data(Qt::UserRole).value<vehicle>();
    car.setCarType(carType);
    this->vehicleTable->insert(car.getPlateNumber(), car);
    emit this->updateVehicle(car);
}

void vehicleRegisterDialog::insertVehicleRow(vehicle car)
{
    int curRow = this->ui->vehicleShowTableWidget->rowCount();
    this->ui->vehicleShowTableWidget->setRowCount(curRow + 1);
    QTableWidgetItem *plateNumberItem = new QTableWidgetItem(car.getPlateNumber());
    plateNumberItem->setFlags(plateNumberItem->flags() & (~Qt::ItemIsEditable));
    QVariant bindData;
    bindData.setValue(car);
    plateNumberItem->setData(Qt::UserRole, bindData);

    this->ui->vehicleShowTableWidget->setItem(curRow, 0, plateNumberItem);
    this->ui->vehicleShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(car.getCarType()));

}

QString vehicleRegisterDialog::transferPlateNumber(QString str)
{
    QString retStr;
    for(int i = 0 ; i < str.length(); i++)
    {
        if(str[i] == 'O' || str[i] == 'o')
        {
            retStr.append("0");
        }
        else if(str[i] == 'I' || str[i] == 'i')
        {
            retStr.append("1");
        }
        else
        {
            retStr.append(str[i]);
        }
    }

    return retStr;
}
