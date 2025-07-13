#include "server.h"
#include "ui_server.h"

#include "commuInfo.h"
#include "createnewroom.h"
#include "chattingroom.h"

#include <QtNetwork>

#include <QTabWidget>
#include <QTabBar>
#include <QMessageBox>
#include <QListWidget>
#include <commuInfo.h>

#include <QMap>

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

    connect(ui->roomList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem* item){
        for(auto& perRoom : rooms){
            if(perRoom.name == item->text()){
                enterRoom(perRoom);
            }
        }
    });
    connect(ui->enterRoomButton, &QPushButton::clicked, this, [=](){
        auto* curRoomItem = ui->roomList->currentItem();
        for(auto& perRoom : rooms){
            if(perRoom.name == curRoomItem->text()){
                enterRoom(perRoom);
            }
        }
    });

    //채팅 받기.
    // connect(this, &Server::ChattingRespond, ui->ChattingPage1->, [=](const CommuInfo& commuInfo){
    //     auto data = commuInfo.GetChat();
    //     ui->Chatting->GetChattingList()->addItem(data.first + QString(" : ") + data.second);
    // });

    userManager.userListJsonLoad();
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
    //if(clientConnection->bytesAvailable() > BLOCK_SIZE) return;
    QByteArray bytearray = clientConnection->readAll();

    //commuInfoQueue.push(CommuInfo{bytearray});
    auto info = CommuInfo{bytearray};
    auto type = info.GetType();
    qDebug("type : %d", type);

    switch(type){
    case CommuType::Infos:
        InfosFetchRespond(info, clientConnection);
        break;
    case CommuType::Chatting:
        emit ChattingRespond(info, clientConnection);
        break;
    case CommuType::InfoFix:
        emit InfosFixRespond(info, clientConnection);
        break;
    case CommuType::InfoAdd:
        AddRespond(info, clientConnection);
        emit InfosAddRespond(info, clientConnection);
        break;
    case CommuType::AUTH:
        AUTHRespond(info, clientConnection);
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

void Server::enterRoom(RoomData &roomData)
{
    ChattingRoom* roomUI = new ChattingRoom();

    roomUI->SetStacked(ui->Chatting);
    roomUI->SetRoomData(&roomData);

    ui->Chatting->addWidget(roomUI);

    ui->Chatting->setCurrentIndex(1);
}

void Server::CreateNew_Room(const RoomData &newData)
{
    rooms.push_back(newData);
    rooms.back().index = rooms.size() - 1;
    ui->roomList->addItem(newData.name);

    enterRoom(const_cast<RoomData&>(newData));
}

void Server::AUTHRespond(const CommuInfo &commuInfo, QTcpSocket* socket)
{
    auto IDAndPwd = commuInfo.GetIDPwd();

    QString ID = IDAndPwd.first;
    QString Pwd = IDAndPwd.second;

    UserInfo* temp = userManager.userSearchById(ID);
    if(temp == nullptr){
        // id 불일치
        qDebug() << "입력한 id 가 유저 리스트에 없습니다.";
        ID = tr("No");
    } else{
        // password 검증
        if(Pwd.compare(temp->getPassword()) == 0){
            // client.ui 가 열렸을 때, server connect 를 통한 새 클라이언트 연결 추가.
            // commuInfo -> client session 관리 리스트에 세션 추가
            ID = temp->getName();
            Pwd = tr("Yes");

            // 해당 클라를 찾아 이름 정해주기.
            for(auto& cla : clients){
                if(cla.socket == socket){
                    cla.name = temp->getName();
                }
            }
        } else {
            // password 불일치
            qDebug() << "계정의 password 가 일치하지 않습니다.";
            Pwd = tr("No");
        }
    }
    QJsonObject whole;
    QJsonObject data;
    data["ID"] = ID;
    data["password"] = Pwd;
    whole["CommuType"] = tr("AUTH");
    whole["Data"] = data;

    QJsonDocument docu(whole);

    QByteArray ba = docu.toJson(QJsonDocument::Compact);
    qDebug("Writee %lld", ba.size());
    socket->write(ba);
}

void Server::AddRespond(const CommuInfo &commuInfo, QTcpSocket *clientConnection)
{
    auto usersToAdd = commuInfo.GetAddingUsers();

    qDebug("AddRespond");

    for(auto user : usersToAdd){
        auto* perUser = new UserInfo(user.getID(), user.getName(), user.getPassword(), user.getEmail(), user.getIsAdmin());
        qDebug() << user.getID() << user.getName() << user.getPassword() << user.getEmail() << user.getIsAdmin();
        userManager.userInsert(perUser);
    }

    //인서트 한다음 세이브.
    userManager.userListJsonSave();
    userManager.userListJsonLoad();
}

void Server::InfosFetchRespond(const CommuInfo &commuInfo, QTcpSocket *clientConnection)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);
    qDebug() << "Pro TYpe : " << productType;
    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        SearchDataResponse(commuInfo, clientConnection);
        break;
    default:
        break;
    }

}

void Server::SearchDataResponse(const CommuInfo& commuInfo, QTcpSocket* socket) {
    ProductInfo::Filter filter;
    auto type = commuInfo.GetRequestProducts(filter);
    QString searchData = filter.keyword;
    int min_price = qMin(filter.minPrice, filter.maxPrice);
    int max_price = filter.maxPrice == 0 ? 9999999 : filter.maxPrice;

    QJsonArray searchResult;

    if(type == ProductInfo::ProductType::Book){
        QMap<QString, Book*> list = this->bookManager.bookListRead();
        for (auto i = list.begin(); i != list.end(); ++i) {
            Book* book = i.value();
            if (book->getPrice() < min_price || book->getPrice() > max_price)
                continue;

            bool matched = false;
            switch (filter.type) {
            case ProductInfo::FilterType::Name:
                matched = book->getName().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Author:
                matched = book->getWriter().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Company:
                matched = book->getCompany().contains(searchData, Qt::CaseInsensitive);
                break;
            }

            if (matched) {
                searchResult.append(book->toJsonObject());
            }
        }
    } else if(type == ProductInfo::ProductType::Music){
        QMap<QString, Music*> list = this->musicmanager.musicListRead();
        for (auto i = list.begin(); i != list.end(); ++i) {
            Music* music = i.value();
            if (music->getPrice() < min_price || music->getPrice() > max_price)
                continue;

            bool matched = false;
            switch (filter.type) {
            case ProductInfo::FilterType::Name:
                matched = music->getName().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Author:
                matched = music->getArtist().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Company:
                matched = music->getCompany().contains(searchData, Qt::CaseInsensitive);
                break;
            }

            if (matched) {
                searchResult.append(music->toJsonObject());
            }
        }
    } else if(type == ProductInfo::ProductType::Blueray){
        QMap<QString, Blueray*> list = this->bluerayManager.bluerayListRead();
        for (auto i = list.begin(); i != list.end(); ++i) {
            Blueray* blueray = i.value();
            if (blueray->getPrice() < min_price || blueray->getPrice() > max_price)
                continue;

            bool matched = false;
            switch (filter.type) {
            case ProductInfo::FilterType::Name:
                matched = blueray->getName().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Author:
                matched = blueray->getArtist().contains(searchData, Qt::CaseInsensitive);
                break;
            case ProductInfo::FilterType::Company:
                matched = blueray->getCompany().contains(searchData, Qt::CaseInsensitive);
                break;
            }

            if (matched) {
                searchResult.append(blueray->toJsonObject());
            }
        }
    }

    CommuInfo responseInfo;
    if(type == ProductInfo::ProductType::Book){
        responseInfo.RequestProducts(ProductInfo::ProductType::Book, filter);
    } else if (type == ProductInfo::ProductType::Music){
        responseInfo.RequestProducts(ProductInfo::ProductType::Music, filter);
    } else if (type == ProductInfo::ProductType::Blueray){
        responseInfo.RequestProducts(ProductInfo::ProductType::Blueray, filter);
    }
    responseInfo.AppendResponseArray(searchResult);

    // base64 이미지 QString 이 포함되어 있어서 bytearray 전체 데이터 전송이 아닌 4 바이트 길이를 먼저 보냄
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)responseInfo.GetByteArray().size(); // 길이 프리픽스
    packet.append(responseInfo.GetByteArray());

    socket->write(packet); // 길이  + 나머지 모든 데이터(response 데이터 포함)
    socket->flush(); // 추가적으로 송신 버퍼를 즉시 밀어줌
    qDebug() << "서버: BookSearchDataResponse 응답 전송 완료";
}
