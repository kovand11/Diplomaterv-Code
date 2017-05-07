#include "linewidget.h"

LineWidget::LineWidget(QString deviceAddress)
{
    this->deviceAddress = deviceAddress;
    timer = new QTimer();
    defaultFont = new QFont("Roboto",16);
}

QLayout *LineWidget::getLayout()
{
    return this->layout;
}

void LineWidget::startPolling(int ms)
{
    timer->setInterval(ms);
    QObject::connect(this->timer,&QTimer::timeout,[&](){
        acquireData();
        updateWidget();
    });
    timer->start();
}

void LineWidget::stopPolling()
{
    timer->stop();
}

LineWidget::~LineWidget()
{
    delete this->timer;
}
