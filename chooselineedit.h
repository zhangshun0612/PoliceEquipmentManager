#ifndef CHOOSELINEEDIT_H
#define CHOOSELINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>
class chooseLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit chooseLineEdit(QWidget *parent = 0);
    ~chooseLineEdit();
signals:
    void lineEditPressed();
public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // CHOOSELINEEDIT_H
