#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widgetListButton,&QToolButton::clicked,this,&MainWindow::onWidgetList);
    connect(ui->settingsButton,&QToolButton::clicked,this,&MainWindow::onSettings);
    connect(ui->programmingButton,&QToolButton::clicked,this,&MainWindow::onProgramming);
    connect(ui->closeButton,&QToolButton::clicked,this,&MainWindow::onClose);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onWidgetList()
{
    ui->pageStack->setCurrentIndex(0);
}

void MainWindow::onSettings()
{
    ui->pageStack->setCurrentIndex(1);
    QProcess *process = new QProcess(this);
    QString file = "matchbox-keyboard";
    process->start(file);
}

void MainWindow::onProgramming()
{
    ui->pageStack->setCurrentIndex(2);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("nut8A5+-");
    db.setDatabaseName("testdb");

    if (db.open())
        qDebug() << "ISOPEN";
    else
        qDebug() << "ISNOTOPEN";

}

void MainWindow::onClose()
{
    this->close();
}
