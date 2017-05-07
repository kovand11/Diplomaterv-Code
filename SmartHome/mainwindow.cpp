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








    debugLineWidget = new DebugLineWidget();
    environmentalSensorWidget = new EnvironmentalSensorWidget("");
    environmentalSensorWidget->setData(23.5f,1025.1f,25.5f,32.4f,25,67,44,115);
    //environmentalSensorWidget->startPolling(100);

    qDebug() << "kutya";

    QStringList serverInfoList;
    serverInfoList.append(serverAddress);
    serverInfoList.append(username);
    serverInfoList.append(password);
    serverInfoList.append(databaseName);


    openDetectorWidget = new OpenDetectorWidget(serverInfoList.join(' '));
    /*openDetectorWidget->processSingleEvent(1551,true,"1991.01.25 22:46");
    openDetectorWidget->processSingleEvent(1551,true,"1991.01.25 22:46");
    openDetectorWidget->processSingleEvent(1551,true,"1991.01.25 22:46");
    openDetectorWidget->processSingleEvent(1551,true,"1991.01.25 22:46");
    openDetectorWidget->processSingleEvent(1551,true,"1991.01.25 22:46");
    openDetectorWidget->createDoor(25411,true);
    openDetectorWidget->createDoor(25451,false);
    openDetectorWidget->createDoor(6585,true);*/

    wifiSocketWidget = new WifiSocketWidget("");



    ui->widgetLayout->addLayout(debugLineWidget->getLayout());
    ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
    ui->widgetLayout->addLayout(environmentalSensorWidget->getLayout());
    ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
    ui->widgetLayout->addLayout(openDetectorWidget->getLayout());
    ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
    ui->widgetLayout->addLayout(wifiSocketWidget->getLayout());

    openDetectorWidget->startPolling(1000);

    debugLineWidget->addText("Application started");

    environmentalSensorWidget->startPolling(1000);

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
