#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include <QDialog>

namespace Ui {
class passwordChangeDialog;
}

class passwordChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit passwordChangeDialog(QString oldpassword, QWidget *parent = 0);
    ~passwordChangeDialog();

    QString getNewManagerPassword();
signals:

private slots:
    void okSlot();
    void changePasswordOk(bool flag);
private:
    Ui::passwordChangeDialog *ui;
    QString oldManagerPassword;
    QString newManagerPassword;

};

#endif // PASSWORDCHANGEDIALOG_H
