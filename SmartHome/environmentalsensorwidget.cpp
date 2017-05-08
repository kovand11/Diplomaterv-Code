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
    QUrl url;
    url.setScheme("http");
    url.setHost("192.168.0.17");
    url.setPath("/json");
    QNetworkRequest request(url);
    networkReply = networkManager.get(request);
    connect(networkReply,&QNetworkReply::finished,[&]()
    {
        QByteArray data = networkReply->readAll();
        qDebug()<< "Raw" << data;
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();
        temperature = object.value("Temp").toString().toFloat();
        pressure = object.value("Pres").toString().toFloat();
        humidity = object.value("Hum").toString().toFloat();
        ambientLight = object.value("Amb").toString().toFloat();
        redCounter = object.value("R").toString().toFloat();
        greenCounter = object.value("G").toString().toFloat();
        blueCounter = object.value("B").toString().toFloat();
        whiteCounter = object.value("W").toString().toFloat();


        if (storageCallback)
        {
            storageCallback(object.value("ID").toString(),
                        object.value("Temp").toString(),
                        object.value("Pres").toString(),
                        object.value("Hum").toString(),
                        object.value("Amb").toString(),
                        object.value("R").toString(),
                        object.value("G").toString(),
                        object.value("B").toString(),
                        object.value("W").toString()
                        );
        }
    });




}

void EnvironmentalSensorWidget::setStorageCallback(std::function<void (QString, QString, QString, QString, QString, QString, QString, QString, QString)> callback)
{
    storageCallback = callback;
}

void EnvironmentalSensorWidget::createWidget()
{
    layout = new QVBoxLayout();

    QHBoxLayout *tempPressHumLayout = new QHBoxLayout();
    QHBoxLayout *lightAndLedLayout = new QHBoxLayout();

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
        setDeveloperParam("blueled",( ch==Qt::Checked ? "1" : "0" ));
    });

    amberCheckbox = new QCheckBox("A");
    connect(amberCheckbox,&QCheckBox::stateChanged,[&](int ch){
        setDeveloperParam("amberled",( ch==Qt::Checked ? "1" : "0" ));
    });

    lightAndLedLayout->addWidget(blueCheckbox);
    lightAndLedLayout->addWidget(amberCheckbox);


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

void EnvironmentalSensorWidget::setDeveloperParam(QString key, QString value)
{
    QUrl url;
    url.setScheme("http");
    url.setHost("192.168.0.17");
    url.setPath("/developer");
    url.setQuery(key + "=" + value);
    qDebug() << "url" << url;
    QNetworkRequest request(url);
    networkManager.get(request);
}
