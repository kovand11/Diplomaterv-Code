#ifndef OPENDETECTORWIDGET_H
#define OPENDETECTORWIDGET_H

#include <QLayout>
#include <QListWidget>
#include <QList>
#include <QLabel>
#include "linewidget.h"


class OpenDetectorWidget : public QObject, public LineWidget
{
public:
    OpenDetectorWidget(QString ip,QObject *parent = 0);

    void acquireData() override;

    void processSingleEvent(int doorId,int isOpen,QString date);

    void createDoor(int id, bool isOpen);

protected:
    void createWidget() override;
    void updateWidget() override;

    void processDatabase();

    void setDoorState(int id, bool isOpen);




    int eventCount;

    QListWidget *eventList;
    QList<QVBoxLayout*> doorLayouts;
    QList<QLabel*> doorIcons;
    QList<QLabel*> doorTexts;
    QList<int> doorIds;
};

#endif // OPENDETECTORWIDGET_H
