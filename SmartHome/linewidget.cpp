#include "linewidget.h"

LineWidget::LineWidget(QString deviceIp)
{
    this->deviceIp = deviceIp;
    timer = new QTimer();
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