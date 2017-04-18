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
    connect(ui->toggleFullscreenButton,&QToolButton::clicked,this,&MainWindow::onToggleFullscreen);
    connect(ui->keyboardButton,&QPushButton::clicked,this,&MainWindow::onKeyboard);

    QSettings settings("AndrasKovacs", "Smart Home");
    serverAddress = settings.value("serverAddress").toString();
    databaseName = settings.value("databaseName").toString();
    username = settings.value("username").toString();
    password = settings.value("password").toString();
    showDebugWidget = settings.value("showDebugWidget").toBool();
    showOpenDetectorDBWidget = settings.value("showOpenDetectorDBWidget").toBool();
    devices = settings.value("devices").toString();
    this->onSettingsReset();




    ui->listWidget->addItem("");
    debugLineWidget = new DebugLineWidget(ui->listWidget,0);
}

MainWindow::~MainWindow()
{
    QSettings settings("AndrasKovacs", "Smart Home");
    settings.setValue("serverAddress",serverAddress);
    settings.setValue("databaseName",databaseName);
    settings.setValue("username",username);
    settings.setValue("password",password);
    settings.setValue("showDebugWidget",showDebugWidget);
    settings.setValue("showOpenDetectorDBWidget",showOpenDetectorDBWidget);
    settings.setValue("devices",devices);
    delete ui;
}

void MainWindow::onWidgetList()
{
    ui->pageStack->setCurrentIndex(0);
}

void MainWindow::onSettings()
{
    ui->pageStack->setCurrentIndex(1);    
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

            queryString += query.value(1).toString() + ", "
                    + query.value(2).toString() + ", "
                    + query.value(3).toString() + "; ";



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

void MainWindow::onToggleFullscreen()
{
    if (this->isFullScreen())
    {
        this->showNormal();
    }
    else
    {
        this->showFullScreen();
    }

}

void MainWindow::onKeyboard()
{
    QProcess *process = new QProcess(this);
    QString file = "matchbox-keyboard";
    process->start(file);
}
