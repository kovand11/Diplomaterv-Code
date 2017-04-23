#ifndef ENVIRONMENTALSENSORWIDGET_H
#define ENVIRONMENTALSENSORWIDGET_H

#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "linewidget.h"

class EnvironmentalSensorWidget : public QObject, public LineWidget
{
    Q_OBJECT
public:
    explicit EnvironmentalSensorWidget(QString ip,QObject *parent = 0);

    QLayout *getLayout();

    void setData(float temperature, float pressure, float humidity, float ambientLight,
                 int redCounter, int greenCounter, int blueCounter, int whiteCounter);

    void acquireData() override;

signals:

public slots:

private:
    void createWidget() override;
    void updateWidget() override;

    QVBoxLayout *layout;

    QLabel *temperatureLabel;
    QLabel *pressureLabel;
    QLabel *humidityLabel;

    QLabel *ambientLightLabel;
    QLabel *redCounterLabel;
    QLabel *greenCounterLabel;
    QLabel *blueCounterLabel;
    QLabel *whiteCounterLabel;


    float temperature;
    float pressure;
    float humidity;

    float ambientLight;
    int redCounter;
    int greenCounter;
    int blueCounter;
    int whiteCounter;


};

#endif // ENVIRONMENTALSENSORWIDGET_H
