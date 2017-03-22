#include "chooselineedit.h"
#include <QDebug>
chooseLineEdit::chooseLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

chooseLineEdit::~chooseLineEdit()
{

}

void chooseLineEdit::mousePressEvent(QMouseEvent *event)
{
    emit this->lineEditPressed();
    return QWidget::mousePressEvent(event);

}
