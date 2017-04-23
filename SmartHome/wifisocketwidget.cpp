#include "wifisocketwidget.h"

WifiSocketWidget::WifiSocketWidget(QString ip,QObject *parent) : QObject(parent), LineWidget(ip)
{
    createWidget();
    isSocketConnected = false;
}

void WifiSocketWidget::acquireData()
{

}

void WifiSocketWidget::createWidget()
{
    layout = new QHBoxLayout();

    connectButton = new QToolButton();
    connectButton->setIcon(QIcon(":/icons/disconnected.png"));
    connectButton->setMinimumSize(64,64);
    connectButton->setIconSize(QSize(48,48));


    connect(connectButton,&QToolButton::clicked,[&](){
        if (isSocketConnected)
            this->connectButton->setIcon(QIcon(":/icons/disconnected.png"));
        else
            this->connectButton->setIcon(QIcon(":/icons/connected.png"));

        isSocketConnected = !isSocketConnected;
    });

    layout->addWidget(connectButton);
    layout->addItem(new QSpacerItem(20,20));
    layout->addWidget(new QLabel("Information about performance"));
}

void WifiSocketWidget::updateWidget()
{

}
