#include "opendetectorwidget.h"

OpenDetectorWidget::OpenDetectorWidget(QString ip,QObject *parent) : QObject(parent), LineWidget(ip)
{
    createWidget();
    processDatabase();
}

void OpenDetectorWidget::acquireData()
{
    //check if there is a new line and process if there is
}

void OpenDetectorWidget::createWidget()
{
    layout = new QHBoxLayout();
    eventList = new QListWidget();
    eventList->setMaximumSize(300,100);
    layout->addWidget(eventList);
    layout->addItem(new QSpacerItem(40,20));
}

void OpenDetectorWidget::updateWidget()
{

}

void OpenDetectorWidget::processDatabase()
{
    qDebug() << "Database process started";
    QSqlQuery query;
    query.exec("SELECT * FROM opendetector");
    while (query.next())
    {
        QString doorId = query.value(1).toString();
        bool isOpen= query.value(2).toString() != "0";
        QString date = query.value(3).toString();
        eventList->addItem(date + ": " + doorId + " " + (isOpen ? "opened" : "closed"));
    }

    query.exec("SELECT DISTINCT(ID) FROM opendetector");
    while (query.next())
    {
        QString idStr = query.value(1).toString();
        int id = idStr.toInt();
        qDebug() << "Distinct door " + QString::number(id);
        doorIds.append(id);
    }

    for (int i = 0; i < doorIds.size(); i++)
    {
        QString idCond = "ID="+QString::number(doorIds.at(i))+ " ";
        query.exec("SELECT OPEN FROM opendetector WHERE " + idCond + "AND (SELECT MAX(TIMESTAMP) FROM opendetector WHERE "+ idCond +")");

        if (query.next())
        {
            bool isOpen= query.value(0).toString() != "0";
            qDebug() << "Created door door " + QString::number(doorIds.at(i)) + " " + (isOpen ? "opened" : "closed");
            createDoor(doorIds.at(i),isOpen);
        }
    }



}

void OpenDetectorWidget::setDoorState(int id, bool isOpen)
{
    int index = doorIds.indexOf(id);
    if (index >= 0 && index < doorIds.size())
    {
        doorIcons.at(index)->setPixmap(( isOpen ? QPixmap(":/icons/unlocked.png") : QPixmap(":/icons/locked.png") ));
    }
    else
    {
        createDoor(id,isOpen);
    }

}

void OpenDetectorWidget::createDoor(int id, bool isOpen)
{
    QVBoxLayout *lay;
    QLabel *ico;
    QLabel *txt;

    lay = new QVBoxLayout();
    ico = new QLabel();
    if (isOpen)
        ico->setPixmap(QPixmap(":/icons/unlocked.png").scaled(32,32));
    else
        ico->setPixmap(QPixmap(":/icons/locked.png").scaled(32,32));
    lay->addWidget(ico);
    txt = new QLabel(QString::number(id)); lay->addWidget(txt);
    doorLayouts.append(lay);
    doorIcons.append(ico);
    doorTexts.append(txt);
    doorIds.append(id);
    layout->addItem(lay);

}

void OpenDetectorWidget::processSingleEvent(int doorId, int isOpen, QString date)
{
    eventList->addItem(date + ": " + QString::number(doorId) + " " + (isOpen ? "opened" : "closed"));
    eventList->scrollToBottom();


}
