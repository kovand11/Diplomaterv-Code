#include "debuglinewidget.h"

DebugLineWidget::DebugLineWidget(QListWidget *listWidget,int widgetRow)
{
    this->listWidget = listWidget;
    this->widgetRow = widgetRow;
}

void DebugLineWidget::setText(QString text)
{
    this->text = text;
    updateWidget();
}

void DebugLineWidget::updateWidget()
{
    listWidget->item(widgetRow)->setText(text);
}
