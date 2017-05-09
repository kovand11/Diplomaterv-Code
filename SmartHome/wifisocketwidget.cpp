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
        {
            this->connectButton->setIcon(QIcon(":/icons/disconnected.png"));
            setControlParam("relay","0");
        }
        else
        {
            this->connectButton->setIcon(QIcon(":/icons/connected.png"));
            setControlParam("relay","1");
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

}

void WifiSocketWidget::setControlParam(QString key,QString value)
{
    QUrl url;
    url.setScheme("http");
    url.setHost("192.168.0.17");
    url.setPath("/control");
    url.setQuery(key + "=" + value);
    QNetworkRequest request(url);
    networkManager.get(request);
}

void WifiSocketWidget::onSet(QString key, QString value)
{
    if (key == "relay")
        setControlParam("relay",value);
}
