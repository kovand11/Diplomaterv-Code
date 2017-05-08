#include "opendetectorwidget.h"

OpenDetectorWidget::OpenDetectorWidget(QString address,QObject *parent) : QObject(parent), LineWidget(address)
{
    createWidget();
    processDatabase();
}

void OpenDetectorWidget::acquireData()
{
    QStringList serverInfoList = deviceAddress.split(' ');

    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(serverInfoList.at(0));
    database.setUserName(serverInfoList.at(1));
    database.setPassword(serverInfoList.at(2));
    database.setDatabaseName(serverInfoList.at(3));
    bool databaseOk = database.open();

    if (!databaseOk)
        return;

    QSqlQuery query;
    query.exec("SELECT MAX(ID) FROM opendetector");
    if (query.next())
    {
        int id = query.value(0).toString().toInt();
        if (id > lastId)
        {
            query.exec("SELECT * FROM opendetector");
            while (query.next())
            {
                lastId = query.value(0).toString().toInt();
                QString doorId = query.value(1).toString();
                bool isOpen= query.value(2).toString() != "0";
                QString date = query.value(3).toString();
                processSingleEvent(doorId.toInt(),isOpen,date);
            }
        }
    }
    //database.close();
    QSqlDatabase::removeDatabase("QMYSQL");
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
    //no further widget update needed with this widget
}

void OpenDetectorWidget::processDatabase()
{
    qDebug() << "Database process started";

    QStringList serverInfoList = deviceAddress.split(' ');

    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(serverInfoList.at(0));
    database.setUserName(serverInfoList.at(1));
    database.setPassword(serverInfoList.at(2));
    database.setDatabaseName(serverInfoList.at(3));
    bool databaseOk = database.open();

    if (!databaseOk)
        return;

    QSqlQuery query;
    query.exec("SELECT * FROM opendetector");
    while (query.next())
    {
        lastId = query.value(0).toString().toInt();
        QString doorId = query.value(1).toString();
        bool isOpen= query.value(2).toString() != "0";
        QString date = query.value(3).toString();
        eventList->addItem(date + ": " + doorId + " " + (isOpen ? "opened" : "closed"));
    }

    QList<int> doors;

    query.exec("SELECT DISTINCT(DEVICE) FROM opendetector");
    while (query.next())
    {
        QString idStr = query.value(0).toString();
        int id = idStr.toInt();
        qDebug() << "Distinct door " + QString::number(id);
        doors.append(id);
    }

    for (int i = 0; i < doors.size(); i++)
    {
        QString idCond = "DEVICE="+QString::number(doors.at(i))+ " ";
        query.exec("SELECT OPEN FROM opendetector WHERE " + idCond + "AND TIMESTAMP=(SELECT MAX(TIMESTAMP) FROM opendetector WHERE "+ idCond +")");

        if (query.next())
        {
            bool isOpen= query.value(0).toString() != "0";
            qDebug() << "Created door door " + QString::number(doors.at(i)) + " " + (isOpen ? "opened" : "closed");
            createDoor(doors.at(i),isOpen);
        }
    }

    database.close();
    QSqlDatabase::removeDatabase("QMYSQL");
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
    if (doorIds.contains(doorId))
    {
        for (int i=0; i<doorIds.size(); i++)
        {
            if (doorId == doorIds.at(i))
            {
                if (isOpen)
                    doorIcons.at(i)->setPixmap(QPixmap(":/icons/unlocked.png").scaled(32,32));
                else
                    doorIcons.at(i)->setPixmap(QPixmap(":/icons/locked.png").scaled(32,32));
            }
        }
    }
    else
    {
        createDoor(doorId,isOpen);
    }
}
