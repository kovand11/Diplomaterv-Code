#include "automationrule.h"

AutomationRule::AutomationRule(QObject *parent) : QObject(parent)
{

}

void AutomationRule::source(QString key, QString value)
{
    Q_UNUSED(key);
    bool ok;
    float v = value.toFloat(&ok);
    if (ok)
    {
        if (v < 20)
        {
            emit setter("blueled","0");
        }
        else
        {
            emit setter("blueled","1");
        }
    }
}
