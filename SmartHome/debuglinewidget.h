#ifndef DEBUGLINEWIDGET_H
#define DEBUGLINEWIDGET_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include <QDateTime>

class DebugLineWidget
{
public:
    DebugLineWidget();
    void addText(QString text);
    QLayout *getLayout();

private:

    QStringList lines;
    int currentLine;

    QLabel* textLabel;
    QLabel* numberLabel;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QHBoxLayout *layout;



};

#endif // DEBUGLINEWIDGET_H
