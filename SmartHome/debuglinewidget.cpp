#include "debuglinewidget.h"

DebugLineWidget::DebugLineWidget()
{
    textLabel = new QLabel("No messages yet.");


    numberLabel = new QLabel ("0/0");
    numberLabel->setMaximumSize(60,40);
    prevButton = new QPushButton("<");
    prevButton->setMaximumSize(40,40);
    nextButton = new QPushButton(">");
    nextButton->setMaximumSize(40,40);
    layout = new QHBoxLayout();
    layout->addWidget(textLabel);
    layout->addWidget(prevButton);
    layout->addWidget(numberLabel);
    layout->addWidget(nextButton);

    prevButton->setEnabled(false);
    nextButton->setEnabled(false);

    QObject::connect (prevButton,&QPushButton::clicked,[&](){
        currentLine--;
        textLabel->setText(lines.at(currentLine));
        numberLabel->setText(QString::number(currentLine+1)+"/"+QString::number(lines.size()));
        if (currentLine == 0)
            prevButton->setEnabled(false);
        else
            prevButton->setEnabled(true);

        if (currentLine == lines.size()-1)
            nextButton->setEnabled(false);
        else
            nextButton->setEnabled(true);

    });

    QObject::connect (nextButton,&QPushButton::clicked,[&](){
        currentLine++;
        textLabel->setText(lines.at(currentLine));
        numberLabel->setText(QString::number(currentLine+1)+"/"+QString::number(lines.size()));
        if (currentLine == 0)
            prevButton->setEnabled(false);
        else
            prevButton->setEnabled(true);

        if (currentLine == lines.size()-1)
            nextButton->setEnabled(false);
        else
            nextButton->setEnabled(true);

    });

}

void DebugLineWidget::addText(QString text)
{
    lines.append(text);
    currentLine = lines.size() - 1;
    textLabel->setText(lines.at(currentLine));
    numberLabel->setText(QString::number(currentLine+1)+"/"+QString::number(lines.size()));
    nextButton->setEnabled(false);
    if (lines.size()!=1)
        prevButton->setEnabled(true);
}

QLayout *DebugLineWidget::getLayout()
{
    return layout;
}


