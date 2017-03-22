#include "policestaffchoosedialog.h"
#include "ui_policestaffchoosedialog.h"

#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(policeStaff)
policeStaffChooseDialog::policeStaffChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::policeStaffChooseDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->policeStaffShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->policeStaffShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->ui->policeStaffShowTableWidget->setColumnCount(3);
    this->ui->policeStaffShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Police Number") << tr("PoliceStaff Name") << tr("Department Name"));
    this->ui->policeStaffShowTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(this->ui->choosePushButton, SIGNAL(clicked()), this, SLOT(choosePushButtonClicked()));

}

policeStaffChooseDialog::~policeStaffChooseDialog()
{
    delete ui;
}

void policeStaffChooseDialog::setPoliceOfficerTable(QList<policeStaff> list)
{
    this->ui->policeStaffShowTableWidget->setRowCount(list.length());

    for(int i = 0 ; i < list.length(); i++)
    {
        policeStaff policeOfficer = list.value(i);
        QTableWidgetItem *policeNumberItem = new QTableWidgetItem(policeOfficer.getPoliceNumber());
        QVariant bindData;
        bindData.setValue(policeOfficer);
        policeNumberItem->setData(Qt::UserRole, bindData);
        this->ui->policeStaffShowTableWidget->setItem(i, 0 , policeNumberItem);
        this->ui->policeStaffShowTableWidget->setItem(i, 1, new QTableWidgetItem(policeOfficer.getStaffName()));
        this->ui->policeStaffShowTableWidget->setItem(i, 2, new QTableWidgetItem(policeOfficer.getDepartmentName()));
    }
}

policeStaff policeStaffChooseDialog::getChoosenOne()
{
    return this->choosenOne;
}


void policeStaffChooseDialog::choosePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->policeStaffShowTableWidget->currentItem();

    int curRow = this->ui->policeStaffShowTableWidget->row(item);

    if(curRow == -1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose a item First"));
        return;
    }
    QTableWidgetItem *policeNumberItem = this->ui->policeStaffShowTableWidget->item(curRow, 0);
    policeStaff policeOfficer = policeNumberItem->data(Qt::UserRole).value<policeStaff>();

    this->choosenOne = policeOfficer;

    this->accept();
}
