#ifndef OPENDETECTORWIDGET_H
#define OPENDETECTORWIDGET_H

#include <QLayout>
#include <QListWidget>
#include <QList>
#include <QLabel>
#include <QDebug>
#include "linewidget.h"

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>


class OpenDetectorWidget : public QObject, public LineWidget
{
public:
    OpenDetectorWidget(QString address,QObject *parent = 0);

    void acquireData() override;

    void processSingleEvent(int doorId,int isOpen,QString date);

    void createDoor(int id, bool isOpen);

protected:
    void createWidget() override;
    void updateWidget() override;

    void processDatabase();

    void setDoorState(int id, bool isOpen);




    int lastId;

    QListWidget *eventList;
    QList<QVBoxLayout*> doorLayouts;
    QList<QLabel*> doorIcons;
    QList<QLabel*> doorTexts;
    QList<int> doorIds;
};

#endif // OPENDETECTORWIDGET_H
