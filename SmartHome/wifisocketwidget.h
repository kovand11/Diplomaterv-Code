#ifndef WIFISOCKETWIDGET_H
#define WIFISOCKETWIDGET_H

#include "linewidget.h"
#include <QToolButton>
#include <QLabel>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>



class WifiSocketWidget : public LineWidget
{
public:
    WifiSocketWidget(QString ip);

    void acquireData() override;
    void createWidget() override;
    void updateWidget() override;
    void setSocketButton(bool on);

    void setData(float power, float voltage, float current, float powerFactor);


    QToolButton *connectButton;
    QLabel *idLabel;
    QLabel *powerLabel;
    QLabel *voltageLabel;
    QLabel *currentLabel;
    QLabel *powerFactorLabel;

    bool isSocketConnected;

private:
    float power;
    float voltage;
    float current;
    float powerFactor;


public slots:
    void onSet(QString key,QString value) override;


};

#endif // WIFISOCKETWIDGET_H
