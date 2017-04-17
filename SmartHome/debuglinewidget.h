#ifndef DEBUGLINEWIDGET_H
#define DEBUGLINEWIDGET_H

#include <QObject>
#include <QListWidget>

class DebugLineWidget
{
public:
    DebugLineWidget(QListWidget *listWidget,int widgetRow);
    void setText(QString text);

private:
    void updateWidget();

    QListWidget* listWidget;
    int widgetRow;

    QString text;



};

#endif // DEBUGLINEWIDGET_H
