#include "recordinquirydialog.h"
#include "ui_recordinquirydialog.h"
#include <QDebug>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

recordInquiryDialog::recordInquiryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recordInquiryDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        this->db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
    }
    this->db.setDatabaseName(DBPATH);

    if(this->db.open())
    {
        qDebug() << "DataBase Open Success";
    }
    else
    {
        qDebug() << "DataBase Open Failed";
    }
    this->ui->inquiryResultShowTableWidget->horizontalHeader()->setStretchLastSection(true);
    this->ui->inquiryResultShowTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    this->ui->beginDateDateEdit->setDate(QDate::currentDate());
    this->ui->endDateDateEdit->setDate(QDate::currentDate());
    connect(this->ui->recordInquiryPushButton, SIGNAL(clicked()), this, SLOT(inquiryPushButtonClicked()));
    connect(this->ui->toExcelPushButton, SIGNAL(clicked()), this, SLOT(toExcelPushButtonClicked()));
}

recordInquiryDialog::~recordInquiryDialog()
{
    this->db.close();
    delete ui;
}

void recordInquiryDialog::inquiryPushButtonClicked()
{
    this->ui->inquiryResultShowTableWidget->clear();
    this->ui->inquiryResultShowTableWidget->setRowCount(0);

    if(this->ui->recordInquiryTypeComboBox->currentIndex() == 0)
    {
        this->ui->inquiryResultShowTableWidget->setColumnCount(7);
        this->ui->inquiryResultShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Lend Date") << tr("Firearm Number") << tr("Firearm Model")
                                                << tr("Lender PoliceNumber") << tr("Lender Name")
                                                << tr("Manager PoliceNumber") <<  tr("Manager Name"));

        QSqlQuery query(this->db);
        query.prepare("select * from " + USAGELOG_TABLE + " where behavior = ? and record_date >= ? and record_date <= ?  order by record_date");
        query.addBindValue("lend");
        query.addBindValue(this->ui->beginDateDateEdit->date());
        query.addBindValue(this->ui->endDateDateEdit->date());

        qDebug() << query.boundValue(1).toString();
        if(query.exec())
        {
            while(query.next())
            {
                QSqlRecord record = query.record();
                int curRow = this->ui->inquiryResultShowTableWidget->rowCount();
                this->ui->inquiryResultShowTableWidget->setRowCount(curRow + 1);

                this->ui->inquiryResultShowTableWidget->setItem(curRow, 0, new QTableWidgetItem(record.value("record_date").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(record.value("firearm_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(record.value("firearm_model").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(record.value("user_police_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(record.value("user_police_name").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 5, new QTableWidgetItem(record.value("manager_police_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 6, new QTableWidgetItem(record.value("manager_police_name").toString()));
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }
    }
    else if(this->ui->recordInquiryTypeComboBox->currentIndex() == 1)
    {
        this->ui->inquiryResultShowTableWidget->setColumnCount(5);
        this->ui->inquiryResultShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Return Date") << tr("Firearm Number") << tr("Firearm Model")
                                                << tr("Returner PoliceNumber") << tr("Returner Name"));

        QSqlQuery query(this->db);
        query.prepare("select * from " + USAGELOG_TABLE + " where behavior = ? and record_date >= ? and record_date <= ?  order by record_date");
        query.addBindValue("return");
        query.addBindValue(this->ui->beginDateDateEdit->date());
        query.addBindValue(this->ui->endDateDateEdit->date());
        if(query.exec())
        {
            while(query.next())
            {
                QSqlRecord record = query.record();
                int curRow = this->ui->inquiryResultShowTableWidget->rowCount();
                this->ui->inquiryResultShowTableWidget->setRowCount(curRow + 1);

                this->ui->inquiryResultShowTableWidget->setItem(curRow, 0, new QTableWidgetItem(record.value("record_date").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(record.value("firearm_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(record.value("firearm_model").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 3, new QTableWidgetItem(record.value("user_police_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 4, new QTableWidgetItem(record.value("user_police_name").toString()));
            }

        }
        else
        {
            qDebug() << query.lastError().text();
        }
    }
    else
    {
        this->ui->inquiryResultShowTableWidget->setColumnCount(3);
        this->ui->inquiryResultShowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Maintenance Date") << tr("Firearm Number") << tr("Firearm Model"));

        QSqlQuery query(this->db);
        query.prepare("select * from " + MAINTENANCELOG_TABLE + " where record_date >= ? and record_date <= ?  order by record_date");
        query.addBindValue(this->ui->beginDateDateEdit->date());
        query.addBindValue(this->ui->endDateDateEdit->date());
        if(query.exec())
        {
            while(query.next())
            {
                QSqlRecord record = query.record();
                int curRow = this->ui->inquiryResultShowTableWidget->rowCount();
                this->ui->inquiryResultShowTableWidget->setRowCount(curRow + 1);

                this->ui->inquiryResultShowTableWidget->setItem(curRow, 0, new QTableWidgetItem(record.value("record_date").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 1, new QTableWidgetItem(record.value("firearm_number").toString()));
                this->ui->inquiryResultShowTableWidget->setItem(curRow, 2, new QTableWidgetItem(record.value("firearm_model").toString()));
            }
        }
        else
        {
            qDebug() << query.lastError().text();
        }
    }
}

void recordInquiryDialog::toExcelPushButtonClicked()
{
    QStringList arglist;
    QString filename;
    if(this->ui->recordInquiryTypeComboBox->currentIndex() == 0)
    {
        filename = QFileDialog::getSaveFileName(this, tr("Open xls file"), tr("./lendRecord.xls"), tr("xls Files(.xls)"));
        arglist << "0" << filename << tr("lendRecord")
                << this->ui->beginDateDateEdit->date().toString("yyyy-MM-dd") << this->ui->endDateDateEdit->date().toString("yyyy-MM-dd");


    }
    else if(this->ui->recordInquiryTypeComboBox->currentIndex() == 1)
    {
        filename = QFileDialog::getSaveFileName(this, tr("Open xls file"), tr("./returnRecord.xls"), tr("xls Files(.xls)"));
        arglist << "1" << filename << tr("returnRecord")
                << this->ui->beginDateDateEdit->date().toString("yyyy-MM-dd") << this->ui->endDateDateEdit->date().toString("yyyy-MM-dd");
    }
    else
    {
        filename = QFileDialog::getSaveFileName(this, tr("Open xls file"), tr("./maintenanceRecord.xls"), tr("xls Files(.xls)"));
        arglist << "2" << filename << tr("maintenanceRecord")
                << this->ui->beginDateDateEdit->date().toString("yyyy-MM-dd") << this->ui->endDateDateEdit->date().toString("yyyy-MM-dd");
    }

    QProcess::execute("RecordDataToExcelFile", arglist);

    QMessageBox::information(this, tr("info"), tr(" Generate Success"));

    //QProcess::execute("explorer", QStringList() << filename);
}

