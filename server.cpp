#include "server.h"
#include "ui_server.h"

#include "commuInfo.h"
#include "createnewroom.h"
#include "chattingroom.h"
#include "MapToVector.h"

#include "bookitem.h"
#include "bluerayitem.h"
#include "musicitem.h"
#include "useritem.h"

#include "addAtUserList.h"

#include "clienthandler.h"

#include <QtNetwork>

#include <QTabWidget>
#include <QTabBar>
#include <QMessageBox>
#include <QListWidget>
#include <commuInfo.h>
#include <QMutexLocker>

#define PORT    5085
#define PENDING_CONN    5
#define MAX_CLIENTS 20

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server), tabWidget(new QTabWidget(this))
{
    tcpServer = new QTcpServer(this);

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
        int i = 0;
        for(auto& perRoom : rooms){
            if(perRoom.name == item->text()){
                enterRoom(i);
                break;
            }
            i++;
        }
    });
    connect(ui->enterRoomButton, &QPushButton::clicked, this, [=](){
        int i = 0;
        auto* curRoomItem = ui->roomList->currentItem();
        if(curRoomItem){
            for(auto& perRoom : rooms){
                if(perRoom.name == curRoomItem->text()){
                    enterRoom(i);
                    break;
                }
                i++;
            }
        }
    });

    connect(ui->getoutRoomButton, &QPushButton::clicked, this, [this]() {
        ui->Chatting->setCurrentIndex(0);
    });

    connect(ui->userListInRoom_All, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item){
        auto* userItem = qobject_cast<UserItem*>(ui->userListInRoom_All->itemWidget(item));
        if(userItem){
            auto* user = const_cast<ClientData*>(userItem->GetPointingClient());
            user->room_idx = currentRoomIndex;
            AddAtUserList(this, ui->userListInRoom_Invited, user);
        }
    });

    connect(this, &Server::signalForBookUI, ui->productsListWidget, [this](const std::vector<Book*>& bookVec){
        ui->productsListWidget->clear();
        for(auto book = bookVec.begin(); book != bookVec.end(); book++){
            BookItem* bookitem = new BookItem(this);
            bookitem->setData(*book);
            QListWidgetItem* item = new QListWidgetItem(ui->productsListWidget);
            item->setSizeHint(bookitem->sizeHint());

            ui->productsListWidget->addItem(item);

            ui->productsListWidget->setItemWidget(item, bookitem);
        }
    });
    connect(this, &Server::signalForMusicUI, ui->productsListWidget, [this](const std::vector<Music*>& musicVec){
        ui->productsListWidget->clear();
        for(auto music : musicVec){
            MusicItem* musicItem = new MusicItem(this);
            musicItem->setData(music);
            QListWidgetItem* item = new QListWidgetItem(ui->productsListWidget);
            item->setSizeHint(musicItem->sizeHint());
            ui->productsListWidget->addItem(item);
            ui->productsListWidget->setItemWidget(item, musicItem);
        }
    });
    connect(this, &Server::signalForBluerayUI, ui->productsListWidget, [this](const std::vector<Blueray*>& bluerayVec){
        ui->productsListWidget->clear();
        for(auto blueray : bluerayVec){
            BluerayItem* bluerayItem = new BluerayItem(this);
            bluerayItem->setData(blueray);
            QListWidgetItem* item = new QListWidgetItem(ui->productsListWidget);
            item->setSizeHint(bluerayItem->sizeHint());
            ui->productsListWidget->addItem(item);
            ui->productsListWidget->setItemWidget(item, bluerayItem);
        }
    });

    connect(ui->searchProductButton, &QPushButton::clicked, this, [this](){
        ProductInfo::ProductType type;
        if(ui->RadioBluray_2->isChecked()){
            type = ProductInfo::Blueray;
        }
        if(ui->RadioBook_2->isChecked()){
            type = ProductInfo::Book;
        }
        if(ui->RadioRecord_2->isChecked()){
            type = ProductInfo::Music;
        }
        this->UpdateUI_Product(type);
    });

    // 초대하기 버튼
    connect(ui->inviteButton, &QPushButton::clicked, this, [this]() {
        auto* selected = ui->userListInRoom_All->currentItem();
        // 클라 고르기
        if(!selected) return;
        UserItem* widget = qobject_cast<UserItem*>(ui->userListInRoom_All->itemWidget(selected));

        auto* cla = const_cast<ClientData*>(widget->GetPointingClient());
        cla->room_idx = currentRoomIndex;
        AddAtUserList(this, ui->userListInRoom_Invited, cla);
    });

    // 강퇴하기 버튼
    connect(ui->outButton, &QPushButton::clicked, this, [this]() {
        auto* selected = ui->userListInRoom_Invited->currentItem();
        UserItem* widget = qobject_cast<UserItem*>(ui->userListInRoom_Invited->itemWidget(selected));

        return;

        //강퇴하기
        auto* cla = const_cast<ClientData*>(widget->GetPointingClient());
        cla->room_idx = -1;
        DeleteAtUserList(ui->userListInRoom_Invited, cla);
    });

    this->userManager = userManager->getInstance();
    this->bluerayManager = bluerayManager->getInstance();
    this->musicManager = musicManager->getInstance();
    this->bookManager = bookManager->getInstance();
    users = MapToVector<UserInfo*>(userManager->userListRead());
    books = MapToVector(this->bookManager->bookListRead());
    bluerays = MapToVector(bluerayManager->bluerayListRead());
    musics = MapToVector(musicManager->musicListRead());
}

Server::~Server()
{
    delete ui;
}

void Server::Initialize()
{
    emit signalForBookUI(books);
    connect(tcpServer, SIGNAL(newConnection()), SLOT(clientConnect()));
}

void Server::clientConnect()
{
    if(clients.size() == MAX_CLIENTS){
        return;
    }

    QTcpSocket* clientConnection = tcpServer->nextPendingConnection();
    QThread* thread = new QThread;
    //클라 목록에 추가.
    clients.push_back(ClientData{-1, tr("관리자"), thread});

    ClientHandler* handler = new ClientHandler(clientConnection);

    handler->moveToThread(thread);

    clientConnection->setParent(nullptr); // 부모-자식 관계 해제
    if(!clientConnection->moveToThread(thread)){
        qDebug() << "넌 못나가";
    }
    else{
        qDebug() << "성공";
    }
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    connect(handler, &ClientHandler::response, this, &Server::respond);
    connect(this, &Server::writeReady, handler, &ClientHandler::send);
    connect(handler, &ClientHandler::disConnected, this, &Server::clientDisconnected);

    thread->start();

    //connect(handler, &ClientHandler::finished, thread, &QThread::quit);

    //QByteArray byteName = clientConnection->readAll();

    // connect(clientConnection, SIGNAL(disconnected()), SLOT(clientDisconnected()));
    // connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLator()));
    // connect(clientConnection, SIGNAL(readyRead()), this, SLOT(respond()));

    qDebug("Connected!");
}

void Server::clientDisconnected(const QThread* thread)
{
    auto found = std::find_if(clients.begin(), clients.end(), [=](const ClientData& cd)->bool{return cd.thread == thread;});
    // 기본 사용자 목록에서 지움
    DeleteAtUserList(ui->userList, &(*found));

    // 방안 사용자 목록(전체)에서 지움
    for(int i = 0; i < ui->userListInRoom_All->count(); ++i){
        QListWidgetItem* item = ui->userListInRoom_All->item(i);
        UserItem* widget = qobject_cast<UserItem*>(ui->userListInRoom_All->itemWidget(item));
        if (widget && widget->GetPointingClient() == &(*found)) {
            ui->userListInRoom_All->takeItem(i);
            break;
        }
    }
    // 방안 사용자 목록(초대받음)에서 지움
    for(int i = 0; i < ui->userListInRoom_Invited->count(); ++i){
        QListWidgetItem* item = ui->userListInRoom_Invited->item(i);
        UserItem* widget = qobject_cast<UserItem*>(ui->userListInRoom_Invited->itemWidget(item));
        if (widget && widget->GetPointingClient() == &(*found)) {
            ui->userListInRoom_Invited->takeItem(i);
            break;
        }
    }
    clients.erase(found);

    qDebug("disconnected");
}

void Server::respond(const QThread* thread, QByteArray bytearray)
{
    //commuInfoQueue.push(CommuInfo{bytearray});
    auto info = CommuInfo{bytearray};

    auto type = info.GetType();
    qDebug() << bytearray;

    qDebug("type : %d", type);
    auto obj{QJsonDocument::fromJson(bytearray).object()};
    qDebug() << obj;

    ClientData* client = nullptr;
    for(auto& cla : clients){
        if(cla.thread == thread){
            client = &cla;
            break;
        }
    }

    switch(type){
    case CommuType::Infos:
        InfosFetchRespond(info, client);
        break;
    case CommuType::Chatting:
        ChattingRespond(info, client);
        break;
    case CommuType::InfoFix:
        emit InfosFixRespond(info, client);
        break;
    case CommuType::InfoAdd:
        AddRespond(info, client);
        emit InfosAddRespond(info, client);
        break;
    case CommuType::AUTH:
        AUTHRespond(info, client);
        break;
    case CommuType::LOGINOUT:
        LoginOutRespond(info, client);
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

void Server::enterRoom(int roomIndex)
{
    currentRoomIndex = roomIndex;
    ui->chattingListInRoom->clear();
    ui->userListInRoom_Invited->clear();
    //초대받은 얘들 리스트에 추가.
    for(auto* cla : rooms[roomIndex].clients){
        AddAtUserList(this, ui->userListInRoom_Invited, const_cast<ClientData*>(cla));
    }

    ui->roomName->setText(rooms[roomIndex].name);
    ui->Chatting->setCurrentIndex(1);
}

void Server::CreateNew_Room(const RoomData &newData)
{
    rooms.push_back(newData);
    rooms.back().index = rooms.size() - 1;
    ui->roomList->addItem(newData.name);


    enterRoom(rooms.size() - 1);
}

void Server::AUTHRespond(const CommuInfo &commuInfo, ClientData* client)
{
    qDebug() << "현재 스레드:" << QThread::currentThread();
    qDebug() << "handler의 소속 스레드:" << client->thread;

    auto IDAndPwd = commuInfo.GetIDPwd();

    QString ID = IDAndPwd.first;
    QString Pwd = IDAndPwd.second;

    UserInfo* temp = userManager->userSearchById(ID);
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

    if(client && client->thread){
        emit writeReady(client->thread, ba);
    }
}

void Server::AddRespond(const CommuInfo &commuInfo, ClientData* client)
{
    auto usersToAdd = commuInfo.GetAddingUsers();

    qDebug("AddRespond");

    for(auto user : usersToAdd){
        auto* perUser = new UserInfo(user.getID(), user.getName(), user.getPassword(), user.getEmail(), user.getIsAdmin());
        qDebug() << user.getID() << user.getName() << user.getPassword() << user.getEmail() << user.getIsAdmin();
        userManager->userInsert(perUser);
    }

    //인서트 한다음 세이브.
    userManager->userListJsonSave();
    userManager->userListJsonLoad();
}

void Server::InfosFetchRespond(const CommuInfo &commuInfo, ClientData* client)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);

}

void Server::LoginOutRespond(const CommuInfo &commuInfo, ClientData* client)
{
    QString clientName;
    CommuInfo confirm;
    bool loginOrOut = commuInfo.GetLoginOrOut(clientName);
    if(loginOrOut){
        // 해당 클라를 찾아 이름 정해주기.
        // 걍 관리자라고 되어있는거 바꾸기.
        client->name = clientName;
        // 채팅 창에 유저 추가.
        AddAtUserList(this, ui->userList, client);
        AddAtUserList(this, ui->userListInRoom_All, client);

        confirm.ServerComfirmLoginOrOut(true);
    }
    else {
        // 채팅 창에 유저 추가.
        for (int i = 0; i < ui->userList->count(); ++i) {
            QListWidgetItem* item = ui->userList->item(i);
            UserItem* widget = qobject_cast<UserItem*>(ui->userList->itemWidget(item));
            if (widget && widget->GetPointingClient() == client) {
                ui->userList->takeItem(i);
                break;
            }
        }
        confirm.ServerComfirmLoginOrOut(false);
    }

    if(client && client->thread){
        emit writeReady(client->thread, confirm.GetByteArray());
    }
}

void Server::ChattingRespond(const CommuInfo &commuInfo, ClientData* client)
{
    auto result = commuInfo.GetChat();
    QString name = result.first;
    QString chat = result.second;
    QString wholeMessage = name + QString(" : ") + chat;
    //클라가 아무 방에도 속해있지 않다.

    QMutex mutex;
    mutex.lock();
    if(client->room_idx == -1){
        ui->generalChattingLog->addItem(wholeMessage);
        // general 채팅 json 파일에 더하는 코드

    }
    else{ //클라가 방에 속해있다.
        rooms[client->room_idx].messages.push_back(wholeMessage);
        if(client->room_idx == currentRoomIndex){
            // 만약 클라이언트가 현재 방에 접속한 경우
            ui->chattingListInRoom->addItem(wholeMessage);
        }
    }
    mutex.unlock();
}

void Server::UpdateUI(Info::InfoType type, ProductInfo::ProductType ifProductType)
{
    switch(type)
    {
        case Info::InfoType::Product:
        UpdateUI_Product(ifProductType);
        break;
    }
}

void Server::UpdateUI_Product(ProductInfo::ProductType productType)
{
    //std::function<bool(const &)>

    switch(productType)
    {
    case ProductInfo::ProductType::Book:
        UpdateUI_Product_Book();
        break;
    case ProductInfo::ProductType::Blueray:
        UpdateUI_Product_Blueray();
        break;
    case ProductInfo::ProductType::Music:
        UpdateUI_Product_Music();
        break;
    case ProductInfo::None:

        break;
    }
}

void Server::UpdateUI_Product_Book()
{
    QString searchKeyword_name = ui->ProductsNameText->toPlainText();
    QString searchKeyword_author = ui->ProductAuthorText->toPlainText();
    QString searchKeyword_company = ui->ProductCompanyText->toPlainText();

    auto bookVec = MapToVector(bookManager->bookListRead());
    std::vector<Book*> bookContain;
    bool nameSearch = true; bool authorSearch = true; bool companySearch = true;
    if(searchKeyword_name.isEmpty()){
        nameSearch = false;
    }
    if(searchKeyword_author.isEmpty()){
        authorSearch = false;
    }
    if(searchKeyword_company.isEmpty()){
        companySearch = false;
    }

    for(auto iter : bookVec){
        bool push = true;
        if(nameSearch && !iter->getName().contains(searchKeyword_name)){
            push = false;
        }
        if(authorSearch && !iter->getWriter().contains(searchKeyword_author)){
            push = false;
        }
        if(companySearch && !iter->getCompany().contains(searchKeyword_company)){
            push = false;
        }
        if(push){
            bookContain.push_back(iter);
        }
    }

    emit signalForBookUI(bookContain);
}

void Server::UpdateUI_Product_Blueray()
{
    QString searchKeyword_name = ui->ProductsNameText->toPlainText();
    QString searchKeyword_author = ui->ProductAuthorText->toPlainText();
    QString searchKeyword_company = ui->ProductCompanyText->toPlainText();

    auto bluerayVec = MapToVector(bluerayManager->bluerayListRead());
    std::vector<Blueray*> bluerayContain;
    bool nameSearch = true; bool authorSearch = true; bool companySearch = true;
    if(searchKeyword_name.isEmpty()){
        nameSearch = false;
    }
    if(searchKeyword_author.isEmpty()){
        authorSearch = false;
    }
    if(searchKeyword_company.isEmpty()){
        companySearch = false;
    }

    for(auto iter : bluerayVec){
        bool push = true;
        if(nameSearch && !iter->getName().contains(searchKeyword_name)){
            push = false;
        }
        if(authorSearch && !iter->getArtist().contains(searchKeyword_author)){
            push = false;
        }
        if(companySearch && !iter->getCompany().contains(searchKeyword_company)){
            push = false;
        }
        if(push){
            bluerayContain.push_back(iter);
        }
    }

    emit signalForBluerayUI(bluerayContain);
}

void Server::UpdateUI_Product_Music()
{
    QString searchKeyword_name = ui->ProductsNameText->toPlainText();
    QString searchKeyword_author = ui->ProductAuthorText->toPlainText();
    QString searchKeyword_company = ui->ProductCompanyText->toPlainText();

    auto musicVec = MapToVector(musicManager->musicListRead());
    std::vector<Music*> musicContain;
    bool nameSearch = true; bool authorSearch = true; bool companySearch = true;
    if(searchKeyword_name.isEmpty()){
        nameSearch = false;
    }
    if(searchKeyword_author.isEmpty()){
        authorSearch = false;
    }
    if(searchKeyword_company.isEmpty()){
        companySearch = false;
    }

    for(auto iter : musicVec){
        bool push = true;
        if(nameSearch && !iter->getName().contains(searchKeyword_name)){
            push = false;
        }
        if(authorSearch && !iter->getArtist().contains(searchKeyword_author)){
            push = false;
        }
        if(companySearch && !iter->getCompany().contains(searchKeyword_company)){
            push = false;
        }
        if(push){
            musicContain.push_back(iter);
        }
    }

    emit signalForMusicUI(musicContain);
}
