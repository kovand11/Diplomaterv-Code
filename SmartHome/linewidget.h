#ifndef LINEWIDGET_H
#define LINEWIDGET_H
#include<QLayout>
#include<QTimer>
#include<QFont>
#include<QMap>
class LineWidget
{
public:
    LineWidget(QString deviceAddress);
    QLayout *getLayout();

    virtual void acquireData() = 0;
    void startPolling(int ms);
    void stopPolling();

    QFont *defaultFont;


    ~LineWidget();

    static QMap<QString,QString> aliases;

signals:
    void notify(QString,QString);

protected:
    QString deviceAddress;
    QLayout *layout;
    QTimer *timer;

    virtual void createWidget() = 0;
    virtual void updateWidget() = 0;
    virtual void onSet(QString key,QString value) = 0;


};

#endif // LINEWIDGET_H
