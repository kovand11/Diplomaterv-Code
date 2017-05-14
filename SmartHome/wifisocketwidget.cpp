#include "wifisocketwidget.h"

WifiSocketWidget::WifiSocketWidget(QString ip) : LineWidget(ip)
{
    createWidget();
    isSocketConnected = false;
    connect(this,&WifiSocketWidget::deviceInfoReady,[&](){idLabel->setText("Socket: " + deviceId);});
}

void WifiSocketWidget::acquireData()
{
    readParameters();
    connect(this,&WifiSocketWidget::parametersReady,[&](){
        power = parameters["power"].toString().toFloat();
        voltage = parameters["voltage"].toString().toFloat();
        current = parameters["current"].toString().toFloat();
        powerFactor = parameters["powerfactor"].toString().toFloat();

        bool isConn = parameters["relay"].toString() == "1";
        setSocketButton(isConn);

    });

}

void WifiSocketWidget::createWidget()
{
    layout = new QHBoxLayout();

    connectButton = new QToolButton();
    connectButton->setIcon(QIcon(":/icons/disconnected.png"));
    connectButton->setMinimumSize(64,64);
    connectButton->setIconSize(QSize(48,48));

    idLabel = new QLabel("Socket: not connected");
    powerLabel = new QLabel("P = 0 W");
    powerLabel->setFont(*defaultFont);
    voltageLabel = new QLabel("U = 0 V");
    currentLabel = new QLabel("I = 0 A");
    powerFactorLabel = new QLabel("fi = 0");



    connect(connectButton,&QToolButton::clicked,[&](){
        isSocketConnected = !isSocketConnected;
        setSocketButton(isSocketConnected);
        writeParameter("relay",isSocketConnected?"1":"0");
    });

    layout->addWidget(idLabel);
    layout->addItem(new QSpacerItem(20,20));
    layout->addWidget(connectButton);
    layout->addItem(new QSpacerItem(20,20));
    layout->addWidget(powerLabel);
    layout->addItem(new QSpacerItem(10,10));
    layout->addWidget(voltageLabel);
    layout->addItem(new QSpacerItem(10,10));
    layout->addWidget(currentLabel);
    layout->addItem(new QSpacerItem(10,10));
    layout->addWidget(powerFactorLabel);




}

void WifiSocketWidget::updateWidget()
{
    powerLabel->setText("P = "+ QString::number(power) +" W");
    voltageLabel->setText("U = "+ QString::number(voltage) +" V");
    currentLabel->setText("I = "+ QString::number(current) +" A");
    powerFactorLabel->setText("fi = "+ QString::number(powerFactor));
}

void WifiSocketWidget::setSocketButton(bool on)
{
    if (on)
        this->connectButton->setIcon(QIcon(":/icons/connected.png"));
    else
        this->connectButton->setIcon(QIcon(":/icons/disconnected.png"));
}

void WifiSocketWidget::setData(float power, float voltage, float current, float powerFactor)
{
    this->power = power;
    this->voltage = voltage;
    this->current = current;
    this->powerFactor = powerFactor;
    updateWidget();
}



void WifiSocketWidget::onSet(QString key, QString value)
{
    if (key == "relay" || key == "led")
        writeParameter(key,value);

}
