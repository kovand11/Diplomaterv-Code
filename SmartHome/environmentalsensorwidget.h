#ifndef ENVIRONMENTALSENSORWIDGET_H
#define ENVIRONMENTALSENSORWIDGET_H

#include <functional>

#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#include "linewidget.h"

class EnvironmentalSensorWidget : public LineWidget
{
    Q_OBJECT
public:
    explicit EnvironmentalSensorWidget(QString ip);

    QLayout *getLayout();

    void setData(float temperature, float pressure, float humidity, float ambientLight,
                 int redCounter, int greenCounter, int blueCounter, int whiteCounter);

    void acquireData() override;

    void setStorageCallback(std::function< void(QString,QString,QString,QString,QString,QString,QString,QString,QString) > callback);


public slots:

private:
    void createWidget() override;
    void updateWidget() override;


    QVBoxLayout *layout;

    QLabel *deviceDescription;

    QLabel *temperatureLabel;
    QLabel *pressureLabel;
    QLabel *humidityLabel;

    QLabel *ambientLightLabel;
    QLabel *redCounterLabel;
    QLabel *greenCounterLabel;
    QLabel *blueCounterLabel;
    QLabel *whiteCounterLabel;

    QCheckBox *blueCheckbox;
    QCheckBox *amberCheckbox;


    float temperature;
    float pressure;
    float humidity;

    float ambientLight;
    int redCounter;
    int greenCounter;
    int blueCounter;
    int whiteCounter;

    std::function< void(QString,QString,QString,QString,QString,QString,QString,QString,QString) > storageCallback; //id, temp, press, hum, amb, r, g, b, w (+implicit date)



public slots:

    void onSet(QString key,QString value) override;



};

#endif // ENVIRONMENTALSENSORWIDGET_H
