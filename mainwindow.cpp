#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTabWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
