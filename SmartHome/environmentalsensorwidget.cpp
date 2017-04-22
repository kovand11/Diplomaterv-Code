#include "environmentalsensorwidget.h"

EnvironmentalSensorWidget::EnvironmentalSensorWidget(QString ip, QObject *parent) :  QObject(parent), LineWidget(ip)
{
    //set data
    createWidget();

}

QLayout *EnvironmentalSensorWidget::getLayout()
{
    return this->layout;
}

void EnvironmentalSensorWidget::setData(float temperature, float pressure, float humidity, float ambientLight, int redCounter, int greenCounter, int blueCounter, int whiteCounter)
{
    //BME280 data
    this->temperature = temperature;
    this->pressure = pressure;
    this->humidity = humidity;

    //VEML6040 data
    this->ambientLight = ambientLight;
    this->redCounter = redCounter;
    this->greenCounter = greenCounter;
    this->blueCounter = blueCounter;
    this->whiteCounter = whiteCounter;
    updateWidget();
}

void EnvironmentalSensorWidget::acquireData()
{
    pressure += 1.0f;
}

void EnvironmentalSensorWidget::createWidget()
{
    layout = new QVBoxLayout();

    QHBoxLayout *tempPressHumLayout = new QHBoxLayout();
    QHBoxLayout *lightAndLedLayout = new QHBoxLayout();

    //Temp, press, hum layout buildup

    QLabel *tempIcon = new QLabel();
    tempIcon->setPixmap(QPixmap(":/icons/temp.png"));
    tempPressHumLayout->addWidget(tempIcon);
    temperatureLabel = new QLabel("28 C");
    tempPressHumLayout->addWidget(temperatureLabel);

    QLabel *pressIcon = new QLabel();
    pressIcon->setPixmap(QPixmap(":/icons/press.png"));
    tempPressHumLayout->addWidget(pressIcon);
    pressureLabel = new QLabel("1025 hPa");
    tempPressHumLayout->addWidget(pressureLabel);

    QLabel *humIcon = new QLabel();
    humIcon->setPixmap(QPixmap(":/icons/hum.png"));
    tempPressHumLayout->addWidget(humIcon);
    humidityLabel = new QLabel("28.5%");
    tempPressHumLayout->addWidget(humidityLabel);


    //Light layout buildup

    QLabel *lightIcon = new QLabel();
    lightIcon->setPixmap(QPixmap(":/icons/light.png"));
    lightAndLedLayout->addWidget(lightIcon);

    ambientLightLabel = new QLabel("25 lux");
    lightAndLedLayout->addWidget(ambientLightLabel);

    redCounterLabel = new QLabel("255");
    lightAndLedLayout->addWidget(redCounterLabel);

    greenCounterLabel = new QLabel("255");
    lightAndLedLayout->addWidget(greenCounterLabel);

    blueCounterLabel = new QLabel("255");
    lightAndLedLayout->addWidget(blueCounterLabel);

    whiteCounterLabel = new QLabel("1000");
    lightAndLedLayout->addWidget(whiteCounterLabel);

    lightAndLedLayout->addSpacing(20);

    lightAndLedLayout->addWidget(new QCheckBox("Blue"));
    lightAndLedLayout->addWidget(new QCheckBox("Amber"));
    QPushButton * refreshButton = new QPushButton("Refresh");
    connect(refreshButton,&QPushButton::clicked,[&](){ this->stopPolling(); });
    lightAndLedLayout->addWidget(refreshButton);


    layout->addLayout(tempPressHumLayout);
    layout->addLayout(lightAndLedLayout);
}

void EnvironmentalSensorWidget::updateWidget()
{
    temperatureLabel->setText(QString::number(temperature) +" C" + QChar(0260) );
    pressureLabel->setText(QString::number(pressure) + " hPa");
    humidityLabel->setText(QString::number(humidity) + "%");

    ambientLightLabel->setText(QString::number(ambientLight) +" lux");
    redCounterLabel->setText("R = " + QString::number(redCounter));
    greenCounterLabel->setText("G = " + QString::number(greenCounter));
    blueCounterLabel->setText("B = " + QString::number(blueCounter));
    whiteCounterLabel->setText("W = " + QString::number(whiteCounter));
}
