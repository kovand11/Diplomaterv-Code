#ifndef WIFISOCKETWIDGET_H
#define WIFISOCKETWIDGET_H

#include "linewidget.h"
#include <QToolButton>
#include <QLabel>


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
};

#endif // WIFISOCKETWIDGET_H
