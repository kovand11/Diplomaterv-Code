#ifndef AUTOMATIONRULE_H
#define AUTOMATIONRULE_H

#include <QObject>
#include <QDebug>

#include <linewidget.h>

class AutomationRule : public QObject
{
    Q_OBJECT
public:
    explicit AutomationRule(LineWidget *src,QString srcParam,QString condition,float constant,QString action,LineWidget *dst,QString dstParam);


signals:
    void setter(QString,QString);

public slots:
    void source(QString key,QString value);

private:
    QString srcParam;
    float constant;
    QString condition;
    QString action;
    QString dstParam;


};

#endif // AUTOMATIONRULE_H
