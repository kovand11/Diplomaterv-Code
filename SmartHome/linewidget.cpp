#include "linewidget.h"

LineWidget::LineWidget(QString deviceAddress)
{
    this->deviceAddress = deviceAddress;
    timer = new QTimer();
    defaultFont = new QFont("Roboto",16);
}

QLayout *LineWidget::getLayout()
{
    return this->layout;
}

void LineWidget::startPolling(int ms)
{
    timer->setInterval(ms);
    QObject::connect(this->timer,&QTimer::timeout,[&](){
        acquireData();
        updateWidget();
    });
    timer->start();
}

void LineWidget::stopPolling()
{
    timer->stop();
}

QString LineWidget::getId()
{
    return deviceId;
}

LineWidget::~LineWidget()
{
    delete this->timer;
}

void LineWidget::getDeviceId()
{
    QUrl url;
    url.setScheme("http");
    url.setHost(deviceAddress);
    url.setPath("/json");
    QNetworkRequest request(url);
    networkReply = networkManager.get(request);
    connect(networkReply,&QNetworkReply::finished,[&]()
    {
        QByteArray data = networkReply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();
        deviceId = object.value("id").toString();
        emit deviceInfoReady();
    });
}

void LineWidget::writeParameter(QString key, QString value)
{
    QUrl url;
    url.setScheme("http");
    url.setHost(deviceAddress);
    url.setPath("/developer");
    url.setQuery(key + "=" + value);
    QNetworkRequest request(url);
    networkManager.get(request);
}

void LineWidget::readParameters()
{
    QUrl url;
    url.setScheme("http");
    url.setHost(deviceAddress);
    url.setPath("/json");
    QNetworkRequest request(url);
    networkReply = networkManager.get(request);
    connect(networkReply,&QNetworkReply::finished,[&]()
    {
        QByteArray data = networkReply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();
        parameters = object.toVariantMap();
        for (QString key : parameters.keys())
        {
            emit notify(key,parameters[key].toString());
            //qDebug() << "notify" << key << parameters[key].toString();
        }
        emit parametersReady();
    });
}

 QMap<QString,QString> LineWidget::aliases;

