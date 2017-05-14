#include "environmentalsensorwidget.h"

EnvironmentalSensorWidget::EnvironmentalSensorWidget(QString ip) : LineWidget(ip)
{
    //set data
    createWidget();
    connect(this,&EnvironmentalSensorWidget::deviceInfoReady,[&](){deviceDescription->setText("Environmental sensor: " + deviceId);});

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

    readParameters();
    connect(this,&EnvironmentalSensorWidget::parametersReady,[&](){
        temperature = parameters["temperature"].toString().toFloat();
        pressure = parameters["pressure"].toString().toFloat();
        humidity = parameters["humidity"].toString().toFloat();
        ambientLight = parameters["ambient"].toString().toFloat();
        redCounter = parameters["red"].toString().toFloat();
        greenCounter = parameters["green"].toString().toFloat();
        blueCounter = parameters["blue"].toString().toFloat();
        whiteCounter = parameters["white"].toString().toFloat();

        bool blueOn = parameters["ledblue"].toString() == "1";
        bool amberOn = parameters["ledamber"].toString() == "1";
        blueCheckbox->setChecked(blueOn);
        amberCheckbox->setChecked(amberOn);
    });
}

void EnvironmentalSensorWidget::setStorageCallback(std::function<void (QString, QString, QString, QString, QString, QString, QString, QString, QString)> callback)
{
    storageCallback = callback;
}

void EnvironmentalSensorWidget::createWidget()
{
    layout = new QVBoxLayout();

    QHBoxLayout *headerLayout = new QHBoxLayout();
    QHBoxLayout *tempPressHumLayout = new QHBoxLayout();
    QHBoxLayout *lightAndLedLayout = new QHBoxLayout();


    //Header buildup

    deviceDescription = new QLabel("Environmental sensor: not connected");
    headerLayout->addWidget(deviceDescription);

    //Temp, press, hum layout buildup

    QLabel *tempIcon = new QLabel();
    tempIcon->setMaximumWidth(64);
    tempIcon->setPixmap(QPixmap(":/icons/temp.png"));
    tempPressHumLayout->addWidget(tempIcon);
    temperatureLabel = new QLabel("-");
    temperatureLabel->setFont(*defaultFont);
    tempPressHumLayout->addWidget(temperatureLabel);


    QLabel *pressIcon = new QLabel();
    pressIcon->setMaximumWidth(64);
    pressIcon->setPixmap(QPixmap(":/icons/press.png"));
    tempPressHumLayout->addWidget(pressIcon);
    pressureLabel = new QLabel("-");
    pressureLabel->setFont(*defaultFont);
    tempPressHumLayout->addWidget(pressureLabel);


    QLabel *humIcon = new QLabel();
    humIcon->setMaximumWidth(64);
    humIcon->setPixmap(QPixmap(":/icons/hum.png"));
    tempPressHumLayout->addWidget(humIcon);
    humidityLabel = new QLabel("-");
    humidityLabel->setFont(*defaultFont);
    tempPressHumLayout->addWidget(humidityLabel);


    //Light layout buildup

    QLabel *lightIcon = new QLabel();
    lightIcon->setPixmap(QPixmap(":/icons/light.png"));
    lightAndLedLayout->addWidget(lightIcon);

    ambientLightLabel = new QLabel("- lux");
    ambientLightLabel->setFont(*defaultFont);

    lightAndLedLayout->addWidget(ambientLightLabel);

    redCounterLabel = new QLabel("-");
    lightAndLedLayout->addWidget(redCounterLabel);

    greenCounterLabel = new QLabel("-");
    lightAndLedLayout->addWidget(greenCounterLabel);

    blueCounterLabel = new QLabel("-");
    lightAndLedLayout->addWidget(blueCounterLabel);

    whiteCounterLabel = new QLabel("-");
    lightAndLedLayout->addWidget(whiteCounterLabel);

    lightAndLedLayout->addSpacing(20);

    blueCheckbox = new QCheckBox("B");
    connect(blueCheckbox,&QCheckBox::stateChanged,[&](int ch){
        writeParameter("ledblue",( ch==Qt::Checked ? "1" : "0" ));
    });

    amberCheckbox = new QCheckBox("A");
    connect(amberCheckbox,&QCheckBox::stateChanged,[&](int ch){
        writeParameter("ledamber",( ch==Qt::Checked ? "1" : "0" ));
    });

    lightAndLedLayout->addWidget(blueCheckbox);
    lightAndLedLayout->addWidget(amberCheckbox);


    layout->addLayout(headerLayout);
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


void EnvironmentalSensorWidget::onSet(QString key, QString value)
{
    if (key == "ledblue" || key == "ledamber")
    {
        writeParameter(key,value);
    }
}
