#include "server.h"
#include "ui_server.h"

#include <QTabWidget>
#include <QTabBar>
#include <QtNetwork>

#define PORT    5085
#define PENDING_CONN    5
#define MAX_CLIENTS 20
#define MAX_ROOMS   5

typedef struct {
    char name[50]; // 접속한 클라이언트 이름
    int room_idx; // 같은 room_idx 끼리 채팅 대화 진행됨 기본 0
} ClientData;

int CLIENT_INDEX = 0;
QTcpSocket* clients[MAX_CLIENTS]; // 접속 클라이언트 소켓

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
        ui->authorLabel->setText(tr("저자"));
        ui->companyLabel->setText(tr("출판사"));
    }
    else if(ui->RadioBluray->isChecked()){
        ui->authorLabel->setText(tr("감독/배우"));
        ui->companyLabel->setText(tr("제작사"));
    }
    else if(ui->RadioRecord->isChecked()){
        ui->authorLabel->setText(tr("아티스트"));
        ui->companyLabel->setText(tr("음반사"));
    }
}
