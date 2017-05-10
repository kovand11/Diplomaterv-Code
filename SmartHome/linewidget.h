#ifndef LINEWIDGET_H
#define LINEWIDGET_H
#include<QLayout>
#include<QTimer>
#include<QFont>
#include<QMap>
#include<QObject>
#include<QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class LineWidget : public QObject
{
Q_OBJECT
public:
    LineWidget(QString deviceAddress);
    QLayout *getLayout();

    virtual void acquireData() = 0;
    void startPolling(int ms);
    void stopPolling();

    QFont *defaultFont;

    QString getId();


    ~LineWidget();

    static QMap<QString,QString> aliases;

signals:
    void notify(QString,QString);
    void parametersReady();
    void deviceInfoReady();

protected:
    QString deviceAddress;
    QString deviceId;
    QLayout *layout;
    QTimer *timer;

    virtual void createWidget() = 0;
    virtual void updateWidget() = 0;

    virtual void getDeviceId();
    virtual void writeParameter(QString key, QString value);
    virtual void readParameters();

    QNetworkAccessManager networkManager;
    QNetworkReply *networkReply;
    QVariantMap parameters;

public slots:
    virtual void onSet(QString key,QString value) = 0;




};

Q_DECLARE_INTERFACE(LineWidget, "LineWidget")

#endif // LINEWIDGET_H
