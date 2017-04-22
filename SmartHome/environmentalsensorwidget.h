#ifndef ENVIRONMENTALSENSORWIDGET_H
#define ENVIRONMENTALSENSORWIDGET_H

#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class EnvironmentalSensorWidget : public QObject
{
    Q_OBJECT
public:
    explicit EnvironmentalSensorWidget(QObject *parent = 0);

    QLayout *getLayout();

    void setData(float temperature, float pressure, float humidity, float ambientLight,
                 int redCounter, int greenCounter, int blueCounter, int whiteCounter);

signals:

public slots:

private:
    void createWidget();
    void updateWidget();

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
