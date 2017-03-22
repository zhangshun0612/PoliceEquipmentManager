#ifndef RECORDINQUIRYDIALOG_H
#define RECORDINQUIRYDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

#include "databasemanager.h"

namespace Ui {
class recordInquiryDialog;
}

class recordInquiryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit recordInquiryDialog(QWidget *parent = 0);
    ~recordInquiryDialog();

private slots:
    void inquiryPushButtonClicked();
    void toExcelPushButtonClicked();
private:
    Ui::recordInquiryDialog *ui;
    QSqlDatabase db;

};

#endif // RECORDINQUIRYDIALOG_H
