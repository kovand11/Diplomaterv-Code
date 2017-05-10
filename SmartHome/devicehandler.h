#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include <QObject>
#include <QList>
#include <linewidget.h>
#include <opendetectorwidget.h>

#include "automationrule.h"

class DeviceHandler : public QObject
{
    Q_OBJECT
public:
    explicit DeviceHandler(QObject *parent = 0);

    OpenDetectorWidget *openDetector;
    QList<AutomationRule*> rules;
    QList<LineWidget*> devices;


signals:

public slots:
};

#endif // DEVICEHANDLER_H
