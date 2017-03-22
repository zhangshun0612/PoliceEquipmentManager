#include "firearmclassregisterdialog.h"
#include "ui_firearmclassregisterdialog.h"

#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(fireArmClass)
fireArmClassRegisterDialog::fireArmClassRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fireArmClassRegisterDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->fireArmClassesShowTableWidget->setColumnCount(2);
    this->ui->fireArmClassesShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("FireArmClassName") << tr("Maintanence Interval"));
    this->ui->fireArmClassesShowTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(this->ui->fireArmClassesShowTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(firearmMaintenanceDayIntervalItemEditting(QTableWidgetItem*)));
    connect(this->ui->fireArmClassesShowTableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(firearmMaintenanceDayIntervalItemEdited(QTableWidgetItem*)));
    connect(this->ui->insertPushButton, SIGNAL(clicked()), this, SLOT(insertPushButtonClicked()));
    connect(this->ui->removePushButton, SIGNAL(clicked()), this, SLOT(removePushButtonClicked()));
}

fireArmClassRegisterDialog::~fireArmClassRegisterDialog()
{
    delete ui;

}

void fireArmClassRegisterDialog::setFireArmClassList(QList<fireArmClass> *list)
{
    this->fireArmClassList = list;
    this->ui->fireArmClassesShowTableWidget->setRowCount(this->fireArmClassList->length());

    for(int i = 0 ; i < this->fireArmClassList->length(); i++)
    {
        fireArmClass fireArmModel = this->fireArmClassList->value(i);
        QTableWidgetItem *classNameItem = new QTableWidgetItem(fireArmModel.getFireArmClassName());
        QVariant bindData;
        bindData.setValue(fireArmModel);
        classNameItem->setData(Qt::UserRole, bindData);
        classNameItem->setFlags(classNameItem->flags() & (~Qt::ItemIsEditable));
        this->ui->fireArmClassesShowTableWidget->setItem(i, 0, classNameItem);
        this->ui->fireArmClassesShowTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(fireArmModel.getMaintenanceDayInterval())));
    }
}

void fireArmClassRegisterDialog::insertPushButtonClicked()
{
    QString firearmClassName = this->ui->fireArmClassNameInputLineEdit->text();
    if(firearmClassName.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Pleas Input a Firearm Name");
        return;
    }

    int firearmMaintenanceDayInterval = this->ui->spinBox->value();

    fireArmClass fireArmModel(firearmClassName, firearmMaintenanceDayInterval);

    if(this->fireArmClassList->contains(fireArmModel) == false)
    {
        emit this->insertFireArmClass(fireArmModel);
        this->fireArmClassList->append(fireArmModel);

        this->ui->fireArmClassesShowTableWidget->setRowCount(this->fireArmClassList->length());
        int curRow = this->fireArmClassList->length() - 1;
        QTableWidgetItem *classNameItem = new QTableWidgetItem(fireArmModel.getFireArmClassName());
        QVariant bindData;
        bindData.setValue(fireArmModel);
        classNameItem->setData(Qt::UserRole, bindData);
        classNameItem->setFlags(classNameItem->flags() & (~Qt::ItemIsEditable));
        this->ui->fireArmClassesShowTableWidget->setItem(curRow, 0, classNameItem);
        this->ui->fireArmClassesShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(QString::number(fireArmModel.getMaintenanceDayInterval())));
    }
}

void fireArmClassRegisterDialog::removePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->fireArmClassesShowTableWidget->currentItem();

    int curRow = this->ui->fireArmClassesShowTableWidget->row(item);

    if(curRow == -1)
        return;

    QTableWidgetItem *classNameItem = this->ui->fireArmClassesShowTableWidget->item(curRow, 0);
    fireArmClass firearmModel = classNameItem->data(Qt::UserRole).value<fireArmClass>();

    emit this->removeFireArmClass(firearmModel);

    this->fireArmClassList->removeOne(firearmModel);

    this->ui->fireArmClassesShowTableWidget->removeRow(curRow);
}

void fireArmClassRegisterDialog::firearmMaintenanceDayIntervalItemEditting(QTableWidgetItem *item)
{
    this->valueBeforeEditting = item->text();
    this->edittingItem = item;
}

void fireArmClassRegisterDialog::firearmMaintenanceDayIntervalItemEdited(QTableWidgetItem *item)
{
    if(this->edittingItem != item)
        return;

    bool okflag;
    int newMaintenanceInterval = item->text().toInt(&okflag);
    if(okflag == false || newMaintenanceInterval < this->ui->spinBox->minimum() || newMaintenanceInterval > this->ui->spinBox->maximum())
    {
        QMessageBox::warning(this, tr("Error"), tr("The Value Input is not Valid"));
        item->setText(this->valueBeforeEditting);
        return;
    }

    int row = this->ui->fireArmClassesShowTableWidget->row(item);
    fireArmClass firearmModel = this->ui->fireArmClassesShowTableWidget->item(row, 0)->data(Qt::UserRole).value<fireArmClass>();
    int index = this->fireArmClassList->indexOf(firearmModel);
    firearmModel.setMaintenanceDayInterval(newMaintenanceInterval);
    if(index != -1)
    {
        this->fireArmClassList->replace(index, firearmModel);
        emit this->updateFireArmClass(firearmModel);
    }
}
