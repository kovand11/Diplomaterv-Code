#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widgetListButton,&QToolButton::clicked,this,&MainWindow::onWidgetList);
    connect(ui->settingsButton,&QToolButton::clicked,this,&MainWindow::onSettings);
    connect(ui->programmingButton,&QToolButton::clicked,this,&MainWindow::onProgramming);
    connect(ui->closeButton,&QToolButton::clicked,this,&MainWindow::onClose);
    connect(ui->applySettingsButton,&QPushButton::clicked,this,&MainWindow::onSettingsApply);
    connect(ui->resetSettingsButton,&QPushButton::clicked,this,&MainWindow::onSettingsReset);




    ui->listWidget->addItem("");
    debugLineWidget = new DebugLineWidget(ui->listWidget,0);
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
    db.setHostName(serverAddress);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(databaseName);
    bool databaseOk = db.open();

    if (databaseOk)
    {
        QString queryString = "";
        QSqlQuery query;
        query.exec("SELECT * FROM opendetector");
        while (query.next()) {
            for (int i=0;i<query.size();i++)
            {
                queryString += query.value(i).toString() + ",";

            }
            queryString += ";";
        }
        debugLineWidget->setText(queryString);
    }
    else
    {
        debugLineWidget->setText("Database error");
    }

    db.close();



}

void MainWindow::onClose()
{
    this->close();
}

void MainWindow::onSettingsApply()
{
    serverAddress = ui->serverAddressEdit->text();
    databaseName = ui->databaseNameEdit->text();
    username = ui->usernameEdit->text();
    password = ui->passwordEdit->text();
    showDebugWidget = ui->showDebugWidgetCheckBox->isChecked();
    showOpenDetectorDBWidget = ui->showOpenDetectorDBWidgetCheckBox->isChecked();
    devices = "";
}

void MainWindow::onSettingsReset()
{
    ui->serverAddressEdit->setText(serverAddress);
    ui->databaseNameEdit->setText(databaseName);
    ui->usernameEdit->setText(username);
    ui->passwordEdit->setText(password);
    ui->showDebugWidgetCheckBox->setChecked(showDebugWidget);
    ui->showOpenDetectorDBWidgetCheckBox->setChecked(showOpenDetectorDBWidget);
    //ui->devicesEdit-
}
