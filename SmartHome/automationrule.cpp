#include "automationrule.h"

AutomationRule::AutomationRule(LineWidget *src,QString srcParam,QString condition,float constant,QString action,LineWidget *dst,QString dstParam)
{
    connect(src,&LineWidget::notify,this,&AutomationRule::source);
    connect(this,&AutomationRule::setter,dst,&LineWidget::onSet);
    this->srcParam = srcParam;
    this->constant = constant;
    this->condition = condition;
    this->action = action;
    this->dstParam = dstParam;
}

void AutomationRule::source(QString key, QString value)
{
    if (key == srcParam)
    {
        bool ok;
        float v = value.toFloat(&ok);
        if (ok)
        {
            bool result = false;
            if (condition == "<")
                result = v < this->constant;
            else if (condition == ">")
                result = v > this->constant;

            qDebug() << "result" << result ;

            if (result)
            {
                if (action == "link" || action == "set")
                    emit setter(dstParam,"1");
            }
            else
            {
                if (action == "link" || action == "reset")
                    emit setter(dstParam,"0");
            }
        }

    }

}
