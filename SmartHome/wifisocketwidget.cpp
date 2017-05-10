#include "wifisocketwidget.h"

WifiSocketWidget::WifiSocketWidget(QString ip) : LineWidget(ip)
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
        {
            this->connectButton->setIcon(QIcon(":/icons/disconnected.png"));
            writeParameter("relay","0");
        }
        else
        {
            this->connectButton->setIcon(QIcon(":/icons/connected.png"));
            writeParameter("relay","1");
        }

        isSocketConnected = !isSocketConnected;
    });

    layout->addWidget(connectButton);
    layout->addItem(new QSpacerItem(20,20));
    QLabel *consumption = new QLabel("0.0 W");
    consumption->setFont(*defaultFont);
    layout->addWidget(consumption);

}

void WifiSocketWidget::updateWidget()
{
    //TODO
}



void WifiSocketWidget::onSet(QString key, QString value)
{
    if (key == "relay")
        writeParameter("relay",value);
}
