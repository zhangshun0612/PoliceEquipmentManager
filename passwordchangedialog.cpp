#include "passwordchangedialog.h"
#include "ui_passwordchangedialog.h"
#include <QMessageBox>

passwordChangeDialog::passwordChangeDialog(QString oldpassword, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwordChangeDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    this->oldManagerPassword = oldpassword;
    connect(this->ui->okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
}

passwordChangeDialog::~passwordChangeDialog()
{
    delete ui;
}

QString passwordChangeDialog::getNewManagerPassword()
{
    return this->newManagerPassword;
}

void passwordChangeDialog::okSlot()
{
    QString oldpassword = this->ui->oldPasswordLineEdit->text();
    QString newpassword1 = this->ui->newPassword1LineEdit->text();
    QString newpassword2 = this->ui->newPassword2LineEdit->text();

    if(oldpassword.isEmpty() || newpassword1.isEmpty() || newpassword2.isEmpty())
    {
       QMessageBox::warning(this, tr("Change Password"), tr("Please Input the corresponding infomation"));
       return;
    }

    if(newpassword1 != newpassword2)
    {
        QMessageBox::warning(this, tr("Change Password"), tr("Two new passwords are not same, Please Check Again"));
        return;
    }

    this->newManagerPassword = newpassword1;
    this->accept();
}

void passwordChangeDialog::changePasswordOk(bool flag)
{
    if(flag)
    {
        QMessageBox::warning(this, tr("Change Password"), tr("Change Password Success"));
        this->accept();
    }
    else
    {
        QMessageBox::warning(this, tr("Change Password"), tr("Change Password Failed"));
    }
}
