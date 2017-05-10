#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>

#include "debuglinewidget.h"
#include "environmentalsensorwidget.h"
#include "opendetectorwidget.h"
#include "wifisocketwidget.h"
#include "automationrule.h"
#include "devicehandler.h"


#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    DebugLineWidget *debugLineWidget;
    OpenDetectorWidget * openDetectorWidget;




    //settings
    QString serverAddress;
    QString databaseName;
    QString username;
    QString password;
    bool showDebugWidget;
    bool showOpenDetectorDBWidget;
    QString devices;

    QSqlDatabase database;

    DeviceHandler deviceHandler;

    LineWidget *getDeviceById(QString id);


private slots:
    void onWidgetList();
    void onSettings();
    void onProgramming();
    void onClose();
    void onSettingsApply();
    void onSettingsReset();
    void onToggleFullscreen();
    void onKeyboard();

    void onNewProgram();
    void loadProgram();
    void saveProgram();






};

#endif // MAINWINDOW_H
