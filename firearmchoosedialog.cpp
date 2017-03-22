#include "firearmchoosedialog.h"
#include "ui_firearmchoosedialog.h"

#include <QMessageBox>
#include <QDebug>
Q_DECLARE_METATYPE(fireArm)
fireArmChooseDialog::fireArmChooseDialog(enum fireArmChooseDialogType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fireArmChooseDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->ui->fireArmShowTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->fireArmShowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);   

    QHeaderView *header = this->ui->fireArmShowTableWidget->horizontalHeader();

    header->setSortIndicator(0, Qt::AscendingOrder);
    header->setSortIndicatorShown(true);
    header->setClickable(true);

    connect(header, SIGNAL(sectionClicked(int)), this->ui->fireArmShowTableWidget, SLOT(sortByColumn(int)));

    dialogType = type;
    if(type == LendInfo)
    {
        this->ui->fireArmShowTableWidget->setColumnCount(2);
        this->ui->fireArmShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Fire Arm Number") << tr("Fire Arm Model"));
    }
    else if(type == ReturnInfo)
    {
        this->ui->fireArmShowTableWidget->setColumnCount(4);
        this->ui->fireArmShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Fire Arm Number") << tr("Fire Arm Model")
                                                                    << tr("LastUser Police Name Number") << tr("LastUser Name"));
    }
    else
    {
        this->ui->fireArmShowTableWidget->setColumnCount(5);
        this->ui->fireArmShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Fire Arm Number") << tr("Fire Arm Model")
                                                                    << tr("LastUser Police Name Number") << tr("LastUser Name")
                                                                    << tr("Is need Maintenance"));

        this->ui->fireArmShowTableWidget->sortByColumn(4, Qt::DescendingOrder);
    }

    this->ui->fireArmShowTableWidget->horizontalHeader()->setStretchLastSection(true);

    connect(this->ui->choosePushButton, SIGNAL(clicked()), this, SLOT(choosePushButtonClicked()));
    connect(this->ui->searchToolButton, SIGNAL(clicked()), this, SLOT(searchPushButtonClicked()));
}

fireArmChooseDialog::~fireArmChooseDialog()
{
    delete ui;
}

void fireArmChooseDialog::setFirearmTable(QList<fireArm> list)
{
    this->ui->fireArmShowTableWidget->setRowCount(list.length());

    for(int i = 0 ; i < list.length(); i++)
    {
        fireArm gun = list.value(i);
        QTableWidgetItem *fireArmItem = new QTableWidgetItem(gun.getFireArmNumber());
        QVariant bindData;
        bindData.setValue(gun);
        fireArmItem->setData(Qt::UserRole, bindData);
        this->ui->fireArmShowTableWidget->setItem(i, 0, fireArmItem);
        this->ui->fireArmShowTableWidget->setItem(i, 1, new QTableWidgetItem(gun.getFireArmModel().getFireArmClassName()));

        if(dialogType == LendInfo)
        {
            //this->ui->fireArmShowTableWidget->setItem(i, 2, new QTableWidgetItem(gun.getFireArmManager().getPoliceNumber()));
            //this->ui->fireArmShowTableWidget->setItem(i, 3, new QTableWidgetItem(gun.getFireArmManager().getStaffName()));
        }
        else if(dialogType == ReturnInfo)
        {
            this->ui->fireArmShowTableWidget->setItem(i, 2, new QTableWidgetItem(gun.getFireArmLastUser().getPoliceNumber()));
            this->ui->fireArmShowTableWidget->setItem(i, 3, new QTableWidgetItem(gun.getFireArmLastUser().getStaffName()));
        }
        else
        {
            this->ui->fireArmShowTableWidget->setItem(i, 2, new QTableWidgetItem(gun.getFireArmLastUser().getPoliceNumber()));
            this->ui->fireArmShowTableWidget->setItem(i, 3, new QTableWidgetItem(gun.getFireArmLastUser().getStaffName()));

            if(gun.isNeedMaintenance())
            {
                this->ui->fireArmShowTableWidget->setItem(i, 4, new QTableWidgetItem(tr("Yes")));
                this->ui->fireArmShowTableWidget->item(i, 4)->setBackground(Qt::red);
            }
            else
            {
                this->ui->fireArmShowTableWidget->setItem(i, 4, new QTableWidgetItem(tr("No")));
            }
        }
    }
}

fireArm fireArmChooseDialog::getChoosenOne()
{
    return this->choosenOne;
}

void fireArmChooseDialog::choosePushButtonClicked()
{
    QTableWidgetItem *item = this->ui->fireArmShowTableWidget->currentItem();

    int curRow = this->ui->fireArmShowTableWidget->row(item);

    if(curRow == -1)
    {
        QMessageBox::warning(this, tr("Error"), tr("Choose a item First"));
        return;
    }
    QTableWidgetItem *fireArmItem = this->ui->fireArmShowTableWidget->item(curRow, 0);
    fireArm gun = fireArmItem->data(Qt::UserRole).value<fireArm>();

    this->choosenOne = gun;

    this->accept();
}

void fireArmChooseDialog::searchPushButtonClicked()
{
    QString searchStr = this->ui->firearmNumberSearchLineEdit->text().trimmed().replace(" ", "");
    bool findFlag = false;
    if(searchStr.isEmpty())
        return;

    for(int i = 0 ; i < this->ui->fireArmShowTableWidget->rowCount(); i++)
    {
        if(this->ui->fireArmShowTableWidget->item(i, 0)->text() == searchStr)
        {
            this->ui->fireArmShowTableWidget->setCurrentCell(i, 0);
            this->ui->fireArmShowTableWidget->item(i, 0)->setSelected(true);
            findFlag = true;
            break;
        }
    }

    if(findFlag == false)
    {
        QMessageBox::information(this, tr("Not Searched"), tr("Can't find the Firearm"));
        return;
    }
}
