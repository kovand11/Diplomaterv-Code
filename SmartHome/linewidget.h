#ifndef LINEWIDGET_H
#define LINEWIDGET_H
#include<QLayout>
#include<QTimer>
#include<QFont>
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

protected:
    QString deviceAddress;
    QLayout *layout;
    QTimer *timer;

    virtual void createWidget() = 0;
    virtual void updateWidget() = 0;
};

#endif // LINEWIDGET_H
