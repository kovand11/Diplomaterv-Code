#ifndef LINEWIDGET_H
#define LINEWIDGET_H
#include<QLayout>
#include<QTimer>
class LineWidget
{
public:
    LineWidget(QString deviceIp);
    QLayout *getLayout();

    virtual void acquireData() = 0;
    void startPolling(int ms);
    void stopPolling();


    ~LineWidget();

protected:
    QString deviceIp;
    QLayout *layout;
    QTimer *timer;

    virtual void createWidget() = 0;
    virtual void updateWidget() = 0;
};

#endif // LINEWIDGET_H
