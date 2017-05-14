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
    connect(ui->saveProgramButton,&QPushButton::clicked,this,&MainWindow::onNewProgram);

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
        deviceHandler.openDetector = openDetectorWidget;
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
                deviceHandler.devices.append(environmentalSensorWidget);
                ui->widgetLayout->addLayout(environmentalSensorWidget->getLayout());
                ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
                environmentalSensorWidget->setData(0,0,0,0,0,0,0,0);
                environmentalSensorWidget->startPolling(1000);

            }
            else if (args[0] == "soc")
            {
                WifiSocketWidget *wifiSocketWidget = new WifiSocketWidget(args[1]);
                deviceHandler.devices.append(wifiSocketWidget);
                ui->widgetLayout->addLayout(wifiSocketWidget->getLayout());
                ui->widgetLayout->addSpacerItem(new QSpacerItem(20,20));
                wifiSocketWidget->setData(0,0,0,0);
                wifiSocketWidget->startPolling(1000);
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



    loadProgram();
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

LineWidget *MainWindow::getDeviceById(QString id)
{
    if (id == "doors")
    {
        return deviceHandler.openDetector;
    }

    for (LineWidget *lw : deviceHandler.devices)
    {
        if (lw->getId() == id)
            return lw;
    }
    return nullptr;
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

void MainWindow::onNewProgram()
{

    QString source = ui->programEdit->toPlainText();
    QStringList lines = source.split('\n');
    for (QString line : lines)
    {
        QStringList params = line.split(' ');
        if (params.size() == 7)
        {
            QString srcDev = params.at(0); LineWidget* srcDevPointer = getDeviceById(srcDev);
            QString srcProp = params.at(1);
            QString cond = params.at(2);
            QString constant = params.at(3); float constantFloat = constant.toFloat();
            QString action = params.at(4);
            QString trgDev = params.at(5); LineWidget* trgDevPointer = getDeviceById(trgDev);
            QString trgProp = params.at(6);
            if (srcDevPointer != nullptr && trgDevPointer != nullptr)
            {
                deviceHandler.rules.append(new AutomationRule(srcDevPointer,srcProp,cond,constantFloat,action,trgDevPointer,trgProp));
            }
            else
            {
                if (srcDevPointer == nullptr)
                {
                    qWarning() << "Error identifying device: " + srcDev;
                    if (debugLineWidget != nullptr)
                        debugLineWidget->addText("Error identifying device: " + srcDev);
                }
                if (trgDevPointer == nullptr)
                {
                    qWarning() << "Error identifying device: " + trgDev;
                    if (debugLineWidget != nullptr)
                        debugLineWidget->addText("Error identifying device: " + trgDev);
                }
            }
        }
        else
        {
            qWarning() << "syntax error at line: " + line;
        }
    }
    saveProgram();
}

void MainWindow::loadProgram()
{
    QSettings settings("AndrasKovacs", "Smart Home");
    QString program = settings.value("program").toString();
    ui->programEdit->setPlainText(program);
}

void MainWindow::saveProgram()
{
    QString program = ui->programEdit->toPlainText();
    QSettings settings("AndrasKovacs", "Smart Home");
    settings.setValue("program",program);
}
