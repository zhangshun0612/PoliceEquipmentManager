#ifndef CALENDARDATEEDIT_H
#define CALENDARDATEEDIT_H

#include <QDateEdit>

class calendarDateEdit : public QDateEdit
{
    Q_OBJECT
public:
    explicit calendarDateEdit(QWidget *parent = 0);
    ~calendarDateEdit();
signals:
    void pressedSignal();
private slots:
    void calendarShow();
    void dateUpdate(QDate date);
public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // CALENDARDATEEDIT_H
