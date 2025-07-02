#include "server.h"
#include "ui_server.h"

#include <QTabWidget>
#include <QTabBar>

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    //ui->tabWidget->tabBar()->setExpanding(true);
}

Server::~Server()
{
    delete ui;
}
