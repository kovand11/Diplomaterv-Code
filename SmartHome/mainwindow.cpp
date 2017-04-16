#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->closeButton,&QToolButton::clicked,this,&MainWindow::onClose);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClose()
{
    this->close();
}
