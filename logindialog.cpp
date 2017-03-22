#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>
loginDialog::loginDialog(QString passwd,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    this->managerPassword = passwd;

    connect(this->ui->okPushButton, SIGNAL(clicked()), this, SLOT(okPushButtonClickedSlot()));
    connect(this->ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelPushButtonClickedSlot()));

    this->setModal(true);
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::okPushButtonClickedSlot()
{
    if(this->ui->passwordLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please enter the password"));
        return;
    }

    if(this->ui->passwordLineEdit->text() != this->managerPassword)
    {
        QMessageBox::warning(this, tr("Error"), tr("Password is not right"));
        return;
    }
    else
    {
        this->accept();
    }
}

void loginDialog::cancelPushButtonClickedSlot()
{
    this->reject();
}
