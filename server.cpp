#include "server.h"
#include "ui_server.h"

#include "commuInfo.h"
#include "createnewroom.h"
#include "chattingroom.h"

#include <QtNetwork>

#include <QTabWidget>
#include <QTabBar>
#include <QMessageBox>

#define PORT    5085
#define PENDING_CONN    5
#define MAX_CLIENTS 20
//#define MAX_ROOMS   5

// typedef struct {
//     char name[50]; // 접속한 클라이언트 이름
//     int room_idx; // 같은 room_idx 끼리 채팅 대화 진행됨 기본 0
// } ClientData;

// int CLIENT_INDEX = 0;
// QTcpSocket* clients[MAX_CLIENTS]; // 접속 클라이언트 소켓

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server), tabWidget(new QTabWidget(this))
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));
    if(!tcpServer->listen(QHostAddress::Any, PORT)){
        QMessageBox::critical(this, tr("Echo Server"), tr("Unable to start the Server: %1.").arg(tcpServer->errorString()));
    }

    ui->setupUi(this);

    //고르기
    connect(ui->RadioBook, &QRadioButton::toggled, this, &Server::transferLabels);
    connect(ui->RadioRecord, &QRadioButton::toggled, this, &Server::transferLabels);
    connect(ui->RadioBluray, &QRadioButton::toggled, this, &Server::transferLabels);

    // 새 방 생성.
    connect(ui->createRoomButton, &QPushButton::clicked, this, [=](){
        CreateNewRoom* cnr = new CreateNewRoom();
        cnr->SetServer(this);
        cnr->show();
    });

    //채팅 받기.
    // connect(this, &Server::ChattingRespond, ui->ChattingPage1->, [=](const CommuInfo& commuInfo){
    //     auto data = commuInfo.GetChat();
    //     ui->Chatting->GetChattingList()->addItem(data.first + QString(" : ") + data.second);
    // });
}

Server::~Server()
{
    delete ui;
}

void Server::clientConnect()
{
    if(clients.size() == MAX_CLIENTS){
        return;
    }

    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    //클라 목록에 추가.
    clients.push_back(ClientData{clientConnection, -1, tr("관리자")});
    //ui->Chatting->AddClientData(&clients.back());

    connect(clientConnection, SIGNAL(disconnected()), SLOT(clientDisconnected()));
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLator()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(respond()));

    qDebug("Connected!");
}

void Server::clientDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket){
        auto found = std::find_if(clients.begin(), clients.end(), [=](const ClientData& cd)->bool{return cd.socket == socket;});
        clients.erase(found);
        //ui->Chatting->DeleteClientData(&(*found));
        qDebug("disconnected");
    }
}

void Server::respond()
{
    QTcpSocket *clientConnection = dynamic_cast<QTcpSocket*>(sender());
    if(clientConnection->bytesAvailable() > BLOCK_SIZE) return;
    QByteArray bytearray = clientConnection->read(BLOCK_SIZE);

    //commuInfoQueue.push(CommuInfo{bytearray});
    auto info = CommuInfo{bytearray};

    auto type = info.GetType();

    switch(type){
    case CommuType::Infos:
        emit InfosFetchRespond(info);
        break;
    case CommuType::Chatting:
        emit ChattingRespond(info);
        break;
    case CommuType::InfoFix:
        emit InfosFixRespond(info);
        break;
    default:
        break;
    }
}

void Server::transferLabels(bool checked)
{
    if(!checked)
        return;

    if(ui->RadioBook->isChecked()){
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

void Server::CreateNew_Room(const RoomData &newData)
{
    rooms.push_back(newData);
    rooms.back().index = rooms.size() - 1;
    ui->roomList->addItem(newData.name);

    ChattingRoom* roomUI = new ChattingRoom();

    roomUI->SetStacked(ui->Chatting);
    roomUI->SetRoomData(const_cast<RoomData*>(&newData));

    ui->Chatting->addWidget(roomUI);

    ui->Chatting->setCurrentIndex(1);
}
