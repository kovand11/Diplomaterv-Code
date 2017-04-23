#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDebug>

#include "debuglinewidget.h"
#include "environmentalsensorwidget.h"
#include "opendetectorwidget.h"

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
    EnvironmentalSensorWidget *environmentalSensorWidget;
    OpenDetectorWidget * openDetectorWidget;


    //settings
    QString serverAddress;
    QString databaseName;
    QString username;
    QString password;
    bool showDebugWidget;
    bool showOpenDetectorDBWidget;
    QString devices;


private slots:
    void onWidgetList();
    void onSettings();
    void onProgramming();
    void onClose();
    void onSettingsApply();
    void onSettingsReset();
    void onToggleFullscreen();
    void onKeyboard();


};

#endif // MAINWINDOW_H
