#include "client.h"
#include "ui_client.h"
#include "tcpcommudefines.h"
#include "chattingroom.h"
#include "commuInfo.h"

#include <QLineEdit>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    qDebug("Client");
    //채팅 받기
    connect(this, &Client::ChattingRespond, ui->chattingTab->GetChattingList(), [=](const CommuInfo& commuInfo){
        auto data = commuInfo.GetChat();
        ui->chattingTab->GetChattingList()->addItem(data.first + QString(" : ") + data.second);
    });
}

Client::~Client()
{
    delete ui;
    clientData.socket->close();
}

void Client::Initialize(QTcpSocket *sock, QStringView Name)
{
    qDebug("Client Initialize");
    clientData.socket = sock;
    clientData.name = Name.toString();

    connect(clientData.socket, SIGNAL(readyRead()), SLOT(respond()));

    ui->chattingTab->AddClientData(&clientData);
}

void Client::respond()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    if(clientSocket->bytesAvailable() > BLOCK_SIZE) return;
    QByteArray bytearray = clientSocket->read(BLOCK_SIZE);

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
