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


class OpenDetectorWidget : public LineWidget
{
public:
    OpenDetectorWidget(QString address);

    void acquireData() override;

    void processSingleEvent(int doorId,int isOpen,QString date);

    void createDoor(int id, bool isOpen);

protected:
    void createWidget() override;
    void updateWidget() override;

    void processDatabase();

    void setDoorState(int id, bool isOpen);

public slots:

    void onSet(QString key,QString value) override;

    void getDeviceId() override;
    void writeParameter(QString key, QString value) override;
    void readParameters() override;






    int lastId;

    QListWidget *eventList;
    QList<QVBoxLayout*> doorLayouts;
    QList<QLabel*> doorIcons;
    QList<QLabel*> doorTexts;
    QList<int> doorIds;
};

#endif // OPENDETECTORWIDGET_H
