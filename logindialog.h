#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QString passwd, QWidget *parent = 0);
    ~loginDialog();

private slots:
    void okPushButtonClickedSlot();
    void cancelPushButtonClickedSlot();
private:
    Ui::loginDialog *ui;
    QString managerPassword;
};

#endif // LOGINDIALOG_H
