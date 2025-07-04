#include "chattingroom.h"
#include "ui_chattingroom.h"

#include <QLineEdit>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QStackedWidget>

ChattingRoom::ChattingRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChattingRoom)
{
    ui->setupUi(this);

    //쓰기
    std::function<void()> func = [=](){
        for(auto clientData : roomData->clients){
            QJsonObject root;
            QJsonObject usr;
            usr["UsrName"] = clientData->name;
            usr["Message"] = ui->inputEdit->text();
            root["CommuType"] = tr("Chatting");

            root["Data"] = usr;

            QJsonDocument doc(root);
            QByteArray array = doc.toJson(QJsonDocument::Compact);

            clientData->socket->write(array);
        }
    };
    connect(ui->inputEdit, &QLineEdit::returnPressed, this, func);
    connect(ui->inputButton, &QPushButton::clicked, this, func);

    connect(ui->getoutButton, &QPushButton::clicked, this, [=](){
        stacked->setCurrentIndex(0);
        stacked->removeWidget(this);
    });
}

ChattingRoom::~ChattingRoom()
{
    delete ui;
}

void ChattingRoom::SetRoomData(RoomData *room)
{
    roomData = room;
    // roomData의 클라이언트 만큼 수 늘이기
    for(auto* cla : room->clients){
        ui->currentClientsWidget->addItem(cla->name);
    }
    // 기록된 채팅 복기
    for(auto message : room->messages){
        ui->chattingList->addItem(message.first + QString(" : ") + message.second);
    }
}

void ChattingRoom::DeleteClientData(const ClientData *data)
{
    auto found = find_if(roomData->clients.begin(), roomData->clients.end(), [=](const ClientData* d){ return data == d; });
    roomData->clients.erase(found);
}

QListWidget *ChattingRoom::GetChattingList()
{
    return ui->chattingList;
}
