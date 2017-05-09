#ifndef AUTOMATIONRULE_H
#define AUTOMATIONRULE_H

#include <QObject>

class AutomationRule : public QObject
{
    Q_OBJECT
public:
    explicit AutomationRule(QObject *parent = 0);


signals:
    void setter(QString,QString);

public slots:
    void source(QString key,QString value);

};

#endif // AUTOMATIONRULE_H
