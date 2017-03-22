#include "calendardateedit.h"

#include <QCalendarWidget>
#include <QDebug>
calendarDateEdit::calendarDateEdit(QWidget *parent) :
    QDateEdit(parent)
{
    connect(this, SIGNAL(pressedSignal()), this, SLOT(calendarShow()));
}

calendarDateEdit::~calendarDateEdit()
{

}

void calendarDateEdit::calendarShow()
{
    QCalendarWidget *calendar = new QCalendarWidget();
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(dateUpdate(QDate)));
    calendar->setSelectedDate(this->date());
    calendar->show();

}

void calendarDateEdit::dateUpdate(QDate date)
{
    this->setDate(date);
}

void calendarDateEdit::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Show Calendar";
    emit this->pressedSignal();

    return QWidget::mousePressEvent(event);
}

void calendarDateEdit::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Show Calendar";
    emit this->pressedSignal();

    return QWidget::keyPressEvent(event);
}
