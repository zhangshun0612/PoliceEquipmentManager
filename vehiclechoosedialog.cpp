#include "vehiclechoosedialog.h"
#include "ui_vehiclechoosedialog.h"

#include <QMessageBox>

Q_DECLARE_METATYPE(vehicle)
vehicleChooseDialog::vehicleChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vehicleChooseDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    QStringList vehicleHeaderLabels;
    vehicleHeaderLabels << tr("Plate Number") << tr("Car Type");
    this->ui->vehicleShowTableWidget->setColumnCount(vehicleHeaderLabels.length());
    this->ui->vehicleShowTableWidget->setHorizontalHeaderLabels(vehicleHeaderLabels);
    this->ui->vehicleShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->vehicleShowTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    this->ui->vehicleShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->vehicleShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(this->ui->choosePushButton, SIGNAL(clicked()), this, SLOT(choosePushButtonClicked()));
}

vehicleChooseDialog::~vehicleChooseDialog()
{
    delete ui;
}

void vehicleChooseDialog::setVehicleTable(QList<vehicle> list)
{
    this->ui->vehicleShowTableWidget->setRowCount(list.length());

    for(int i = 0 ; i < list.length(); i++)
    {
        vehicle car = list.value(i);

        QTableWidgetItem *plateNumberItem = new QTableWidgetItem(car.getPlateNumber());
        QVariant bindData;
        bindData.setValue(car);
        plateNumberItem->setData(Qt::UserRole, bindData);
        this->ui->vehicleShowTableWidget->setItem(i, 0, plateNumberItem);
        this->ui->vehicleShowTableWidget->setItem(i, 1, new QTableWidgetItem(car.getCarType()));

    }
}

vehicle vehicleChooseDialog::getChooseOne()
{
    return this->chooseOne;
}

void vehicleChooseDialog::choosePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->vehicleShowTableWidget->currentItem();

    int curRow = this->ui->vehicleShowTableWidget->row(item);

    if(curRow == -1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose a item First"));
        return;
    }

    QTableWidgetItem *plateNumberItem = this->ui->vehicleShowTableWidget->item(curRow, 0);
    vehicle car = plateNumberItem->data(Qt::UserRole).value<vehicle>();
    this->chooseOne = car;
    this->accept();
}
