#include "opendetectorwidget.h"

OpenDetectorWidget::OpenDetectorWidget(QString ip,QObject *parent) : QObject(parent), LineWidget(ip)
{
    createWidget();
}

void OpenDetectorWidget::acquireData()
{
    eventList->addItem("Some event");
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
