#include "server.h"
#include "ui_server.h"

#include <QTabWidget>
#include <QTabBar>

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    connect(ui->RadioBook, &QRadioButton::toggled, this, &Server::transferLabels);
    connect(ui->RadioRecord, &QRadioButton::toggled, this, &Server::transferLabels);
    connect(ui->RadioBluray, &QRadioButton::toggled, this, &Server::transferLabels);
}

Server::~Server()
{
    delete ui;
}

void Server::transferLabels(bool checked)
{
    if(!checked)
        return;

    if(ui->RadioBook->isChecked()){
        //ui->nameLabel->setText("")
        ui->authorLabel->setText("저자");
        ui->companyLabel->setText("출판사");
    }
    else if(ui->RadioBluray->isChecked()){
        ui->authorLabel->setText("감독/배우");
        ui->companyLabel->setText("제작사");
    }
    else if(ui->RadioRecord->isChecked()){
        ui->authorLabel->setText("아티스트");
        ui->companyLabel->setText("음반사");
    }
}
