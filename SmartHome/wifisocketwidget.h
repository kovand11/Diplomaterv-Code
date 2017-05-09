#ifndef WIFISOCKETWIDGET_H
#define WIFISOCKETWIDGET_H

#include "linewidget.h"
#include <QToolButton>
#include <QLabel>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>



class WifiSocketWidget : public QObject, public LineWidget
{
public:
    WifiSocketWidget(QString ip,QObject *parent = 0);

    void acquireData() override;
    void createWidget() override;
    void updateWidget() override;

    QToolButton *connectButton;

    bool isSocketConnected;
    float power;

private:
    void setControlParam(QString key,QString value);

    QNetworkAccessManager networkManager;
    QNetworkReply *networkReply;
    void onSet(QString key,QString value) override;


};

#endif // WIFISOCKETWIDGET_H
