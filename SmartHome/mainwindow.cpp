#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    debugLineWidget = nullptr;
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

    if (showDebugWidget)
    {
        debugLineWidget = new DebugLineWidget();
        ui->widgetLayout->addLayout(debugLineWidget->getLayout());
        ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
        if (debugLineWidget != nullptr)
            debugLineWidget->addText("Debug line widget added");
    }

    if (showOpenDetectorDBWidget)
    {
        QStringList serverInfoList;
        serverInfoList.append(serverAddress);
        serverInfoList.append(username);
        serverInfoList.append(password);
        serverInfoList.append(databaseName);
        openDetectorWidget = new OpenDetectorWidget(serverInfoList.join(' '));
        ui->widgetLayout->addLayout(openDetectorWidget->getLayout());
        ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
        openDetectorWidget->startPolling(1000);
        if (debugLineWidget != nullptr)
            debugLineWidget->addText("Open detector widget added (" + serverAddress +" : "+ databaseName + ")");
    }

    QStringList devicesList = devices.split('\n');

    for (QString s : devicesList)
    {
        if (debugLineWidget != nullptr)
            debugLineWidget->addText(s);

        if(s != "")
        {
            QStringList args = s.split(' ');
            if (args[0] == "env")
            {
                EnvironmentalSensorWidget *environmentalSensorWidget = new EnvironmentalSensorWidget(args[1]);
                lineWidgets.push_back(environmentalSensorWidget);
                ui->widgetLayout->addLayout(environmentalSensorWidget->getLayout());
                ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
                environmentalSensorWidget->setData(0,0,0,0,0,0,0,0);
                environmentalSensorWidget->startPolling(2000);

            }
            else if (args[0] == "soc")
            {
                WifiSocketWidget *wifiSocketWidget = new WifiSocketWidget(args[1]);
                lineWidgets.push_back(wifiSocketWidget);
                ui->widgetLayout->addLayout(wifiSocketWidget->getLayout());
                ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
            }
            else if (args[0] == "alias")
            {
                QString key = args[1];
                args.removeAt(1);
                args.removeAt(0);
                QString value = args.join(' ');
                LineWidget::aliases.insert(key,value);
            }
        }

    }


    if (debugLineWidget != nullptr)
        debugLineWidget->addText("Application started correctly");

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
    devices = ui->devicesEdit->toPlainText();;
}

void MainWindow::onSettingsReset()
{
    ui->serverAddressEdit->setText(serverAddress);
    ui->databaseNameEdit->setText(databaseName);
    ui->usernameEdit->setText(username);
    ui->passwordEdit->setText(password);
    ui->showDebugWidgetCheckBox->setChecked(showDebugWidget);
    ui->showOpenDetectorDBWidgetCheckBox->setChecked(showOpenDetectorDBWidget);
    ui->devicesEdit->setPlainText(devices);
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
