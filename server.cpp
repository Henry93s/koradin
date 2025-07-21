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

#include "chattinglogwidget.h"

#include <QtNetwork>

#include <QTabWidget>
#include <QTabBar>
#include <QMessageBox>
#include <QListWidget>
#include <commuInfo.h>
#include <QMutexLocker>

#include <QMap>

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
        for(auto& cla : clients){
            for(auto& doub : cla.listAndItem){
                if(doub.item == item){
                    cla.room_idx = currentRoomIndex;
                    AddAtUserList(this, ui->userListInRoom_Invited, &cla);
                    break;
                }
            }
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
        for(auto& cla : clients){
            for(auto& doub : cla.listAndItem){
                if(doub.item == selected){
                    cla.room_idx = currentRoomIndex;
                    AddAtUserList(this, ui->userListInRoom_Invited, &cla);
                }
            }
        }
    });

    // 강퇴하기 버튼
    connect(ui->outButton, &QPushButton::clicked, this, [this]() {
        auto* selected = ui->userListInRoom_Invited->currentItem();
        if(!selected) return;
        for(auto& cla : clients){
            for(auto& doub : cla.listAndItem){
                if(doub.item == selected){
                    cla.room_idx = -1;
                    DeleteAtUserList(ui->userListInRoom_Invited, &cla);
                }
            }
        }
    });

    this->userManager = userManager->getInstance();
    this->bluerayManager = bluerayManager->getInstance();
    this->musicManager = musicManager->getInstance();
    this->bookManager = bookManager->getInstance();
    this->orderManager = orderManager->getInstance();
    this->logManager = logManager->getInstance();

    users = MapToVector<UserInfo*>(userManager->userListRead());
    books = MapToVector(this->bookManager->bookListRead());
    bluerays = MapToVector(bluerayManager->bluerayListRead());
    musics = MapToVector(musicManager->musicListRead());

    // logger
    logManager->getTimeStamp_and_write(LogManager::LogType::INFO, "koradin Server Open");
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
    clients.push_back(ClientData{-1, tr("-"), thread});

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

    qDebug("Connected!");
}

void Server::clientDisconnected(const QThread* thread)
{
    qDebug() << "clientDisconnected";
    auto found = std::find_if(clients.begin(), clients.end(), [=](const ClientData& cd)->bool{return cd.thread == thread;});
    // 기본 사용자 목록에서 지움
    DeleteAtUserList(ui->userList, &(*found));

    // 방안 사용자 목록(전체)에서 지움
    DeleteAtUserList(ui->userListInRoom_All, &(*found));
    // 방안 사용자 목록(초대받음)에서 지움
    DeleteAtUserList(ui->userListInRoom_Invited, &(*found));

    clients.erase(found);

    qDebug("disconnected");
}

void Server::respond(const QThread* thread, QByteArray bytearray)
{
    //commuInfoQueue.push(CommuInfo{bytearray});
    auto info = CommuInfo{bytearray};
    auto type = info.GetType();

    // auto obj{QJsonDocument::fromJson(bytearray).object()};

    // qDebug() << obj;

    ClientData* client = nullptr;
    for(auto& cla : clients){
        if(cla.thread == thread){
            client = &cla;
            break;
        }
    }

    // qDebug() << "type : " << type;

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
    case CommuType::OrderInfos:
        OrderInfosFetchRespond(info, client);
        break;
    case CommuType::OrderAdd:
        OrderAddRespond(info, client);
        break;
    case CommuType::OrderDelete:
        OrderDeleteRespond(info, client);
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
    // qDebug() << "현재 스레드:" << QThread::currentThread();
    // qDebug() << "handler의 소속 스레드:" << client->thread;

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

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)ba.size(); // 길이 프리픽스

    packet.append(ba);
    ba = packet;

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
        // qDebug() << user.getID() << user.getName() << user.getPassword() << user.getEmail() << user.getIsAdmin();
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
    // qDebug() << "Pro TYpe : " << productType;
    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        SearchDataResponse(commuInfo, client);
        break;
    default:
        break;
    }

}

void Server::OrderInfosFetchRespond(const CommuInfo &commuInfo, ClientData* client)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);
    qDebug() << "order_Prod type : " << productType;
    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        SearchOrderDataResponse(commuInfo, client);
        break;
    default:
        break;
    }
}

void Server::OrderAddRespond(const CommuInfo &commuInfo, ClientData* client)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);
    qDebug() << "order_Prod type : " << productType;
    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        AddOrderDataResponse(commuInfo, client);
        break;
    default:
        break;
    }
}

void Server::OrderDeleteRespond(const CommuInfo &commuInfo, ClientData* client)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);
    qDebug() << "delete order_Prod type : " << productType;
    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        DeleteOrderDataResponse(commuInfo, client);
        break;
    default:
        break;
    }
}


void Server::LoginOutRespond(const CommuInfo &commuInfo, ClientData* client)
{
    QString clientName;
    QString clientID;
    CommuInfo confirm;

    std::vector<QString> clientNames;

    bool loginOrOut = commuInfo.GetLoginOrOut(clientName, clientID);
    if(loginOrOut){
        // 해당 클라를 찾아 이름 정해주기.
        // 걍 관리자라고 되어있는거 바꾸기.
        client->name = clientName;
        client->ID = clientID;
        // 채팅 창에 유저 추가.
        AddAtUserList(this, ui->userList, client);
        AddAtUserList(this, ui->userListInRoom_All, client);

        for(auto& cla : clients){
            clientNames.push_back(cla.name);
        }
        confirm.ServerComfirmLoginOrOut(true, clientNames);
    }
    else {
        qDebug() << "clientLogOut";
        // 채팅 창에 유저 삭제.
        DeleteAtUserList(ui->userList, client);
        DeleteAtUserList(ui->userListInRoom_All, client);

        // 현 남은 클라이언트들 담아 보내기.
        for(auto& cla : clients){
            if(&cla == client){     // 만약 로그아웃한 친구면 빼기
                continue;
            }
            clientNames.push_back(cla.name);
        }
        // CommuInfo 조립
        confirm.ServerComfirmLoginOrOut(false, clientNames);
    }

    // base64 이미지 QString 이 포함되어 있어서 bytearray 전체 데이터 전송이 아닌 4 바이트 길이를 먼저 보냄
    confirm.AddSizePacket();

    // 모든 클라에게 보내기.
    for(auto& cla : clients){
        if(cla.thread){
            emit writeReady(cla.thread, confirm.GetByteArray());
        }
    }
}

void Server::ChattingRespond(const CommuInfo &commuInfo, ClientData* client)
{
    ChattingType chatType; QString counterName; QByteArray fileBytes; QString fileName;
    auto result = commuInfo.GetChat(chatType, counterName, fileBytes, fileName);
    QString name = result.first;
    QString chat = result.second;

    QString wholeMessage = name + QString(" : ") + chat;
    //클라가 아무 방에도 속해있지 않다.

    QMutex mutex;
    mutex.lock();
    if(client->room_idx == -1){
        // 클라이언트가 방에 없을 경우.
        AddAtChattingList(this, ui->generalChattingLog, result.first, result.second, fileBytes, fileName);

        // general 채팅 json 파일에 더하는 코드
    }
    else{ //클라가 방에 속해있다.
        rooms[client->room_idx].messages.push_back(wholeMessage);
        if(client->room_idx == currentRoomIndex){
            // 만약 클라이언트가 현재 방에 접속한 경우
            AddAtChattingList(this, ui->chattingListInRoom, result.first, result.second, fileBytes, fileName);
        }
        // 클라 방의 json 파일에 더하는 코드
    }
    mutex.unlock();

    // 나머지 클라이언트에게도 메시지 보내기.
    CommuInfo forClients;
    forClients.SetChat(name, chat, fileBytes, fileName);
    forClients.AddSizePacket();

    // 모두에게 보내기.
    if(chatType == ChattingType::General_ForAdmin)
    {
        for(auto& cla : clients){
            if(cla.thread){
                emit writeReady(cla.thread, forClients.GetByteArray());
            }
        }
    }
    // 귓속말로 한 사람 그리고 보낸 사람에게만 보내기.
    else if(chatType == ChattingType::Whisper)
    {
        for(auto& cla : clients){
            if(cla.thread && (cla.name == counterName || &cla == client)){
                emit writeReady(cla.thread, forClients.GetByteArray());
            }
        }
    }
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


// 도서 or 블루레이 or 음반 상품을 조회하여 json + 데이터의 길이(header) 를 같이 클라이언트에 전달함
void Server::SearchDataResponse(const CommuInfo& commuInfo, ClientData* client) {
    ProductInfo::Filter filter;
    auto type = commuInfo.GetRequestProducts(filter);
    QString searchData = filter.keyword;
    int min_price = qMin(filter.minPrice, filter.maxPrice);
    int max_price = filter.maxPrice == 0 ? 9999999 : filter.maxPrice;

    QJsonArray searchResult;

    if(type == ProductInfo::ProductType::Book){
        QMap<QString, Book*> list = this->bookManager->bookListRead();
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
            case ProductInfo::FilterType::UUID:
                matched = book->getUuid().compare(searchData, Qt::CaseInsensitive) == 0 \
                              ? true : false;
                break;
            }

            if (matched) {
                searchResult.append(book->toJsonObject());
            }
        }
    } else if(type == ProductInfo::ProductType::Music){
        QMap<QString, Music*> list = this->musicManager->musicListRead();
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
            case ProductInfo::FilterType::UUID:
                matched = music->getUuid().compare(searchData, Qt::CaseInsensitive) == 0 \
                              ? true : false;
                break;
            }

            if (matched) {
                searchResult.append(music->toJsonObject());
            }
        }
    } else if(type == ProductInfo::ProductType::Blueray){
        QMap<QString, Blueray*> list = this->bluerayManager->bluerayListRead();
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
            case ProductInfo::FilterType::UUID:
                matched = blueray->getUuid().compare(searchData, Qt::CaseInsensitive) == 0 \
                              ? true : false;
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

    emit writeReady(client->thread, packet);
    // socket->write(packet); // 길이  + 나머지 모든 데이터(response 데이터 포함)
    // socket->flush(); // 추가적으로 송신 버퍼를 즉시 밀어줌
    qDebug() << "서버: SearchDataResponse 응답 전송 완료";
}

void Server::SearchOrderDataResponse(const CommuInfo& commuInfo, ClientData* client) {
    ProductInfo::Filter filter;
    auto type = commuInfo.GetRequestProducts(filter);
    QString searchData = filter.keyword;
    int min_price = qMin(filter.minPrice, filter.maxPrice);
    int max_price = filter.maxPrice == 0 ? 9999999 : filter.maxPrice;

    QJsonArray searchResult;
    QMap<UserInfo*, QVector<ProductInfo*>> orderList;
    orderList = this->orderManager->getOrderList();

    UserInfo* user = this->userManager->userSearchById(client->ID);

    for(auto order = orderList.begin(); order != orderList.end(); ++order){
        if(order.key() == user){
            //qDebug() << "SearchOrderDataResponse() - order.key() == user" << order.key() << "/" << user;
            QVector<ProductInfo*> order_products = order.value();
            QJsonObject obj;
            obj["userID"] = user->getID();
            QJsonArray orderArray;
            int priceSum = 0;

            for(ProductInfo* order_product : order_products){
                if(order_product->getPrice() < min_price || order_product->getPrice() > max_price){
                    continue;
                }
                bool matched = false;
                matched = order_product->getName().contains(searchData, Qt::CaseInsensitive);
                if(matched){
                    QJsonObject itemObj;
                    itemObj["uuid"] = order_product->getUuid();
                    priceSum += order_product->getPrice();
                    orderArray.append(itemObj);
                }
            }
            obj["orderItems"] = orderArray;
            obj["priceSum"] = priceSum;
            searchResult.append(obj);
        }
    }

    CommuInfo responseInfo;
    if(type == ProductInfo::ProductType::Book){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Book, filter, QString("OrderInfos"));
    } else if (type == ProductInfo::ProductType::Music){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Music, filter, QString("OrderInfos"));
    } else if (type == ProductInfo::ProductType::Blueray){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Blueray, filter, QString("OrderInfos"));
    }
    responseInfo.AppendResponseArray(searchResult);

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)responseInfo.GetByteArray().size(); // 길이 프리픽스
    packet.append(responseInfo.GetByteArray());

    emit writeReady(client->thread, packet);
    // socket->write(packet); // 길이  + 나머지 모든 데이터(response 데이터 포함)
    // socket->flush(); // 추가적으로 송신 버퍼를 즉시 밀어줌
    qDebug() << "서버: SearchOrderDataResponse 응답 전송 완료";
}

void Server::AddOrderDataResponse(const CommuInfo& commuInfo, ClientData* client) {
    ProductInfo::Filter filter;
    ProductInfo::ProductType type = commuInfo.GetRequestProducts(filter);
    QString uuid = filter.keyword;

    // 유저 정보 찾기
    UserInfo* user = this->userManager->userSearchById(client->ID);
    if (!user) {
        qDebug() << "서버: 유저를 찾을 수 없습니다.";
        return;
    }

    // 전체 상품 목록에서 해당 UUID를 가진 상품 찾기
    ProductInfo* product = nullptr;
    if (type == ProductInfo::ProductType::Book) {
        product = this->bookManager->bookSearchByUuid(uuid);
    } else if (type == ProductInfo::ProductType::Music) {
        product = this->musicManager->musicSearchByUuid(uuid);
    } else if (type == ProductInfo::ProductType::Blueray) {
        product = this->bluerayManager->blueraySearchByUuid(uuid);
    }
    if (!product) {
        qDebug() << "서버: 해당 uuid 상품 없음";
        return;
    }
    qDebug() << "server addorderdata product -> " << product->getName();

    // 주문 목록에 추가
    int resultAdd = this->orderManager->addOrderList(user, product);
    // 클라이언트에게 주문 응답
    QJsonObject responseObj;
    responseObj["uuid"] = uuid;
    responseObj["userID"] = user->getID();
    if(resultAdd == -1){
        qDebug() << "user 또는 product 없음";
        responseObj["status"] = "fail";
        responseObj["message"] = "user 또는 product 없음";
    } else if(resultAdd == 1){
        qDebug() << "product uuid 중복";
        responseObj["status"] = "fail";
        responseObj["message"] = "product uuid 중복";
    } else if(resultAdd == 0){
        qDebug() << "서버: 주문이 성공적으로 추가되었습니다. UUID:" << uuid << "유저:" << user->getID();
        responseObj["status"] = "success";
        responseObj["message"] = "주문이 완료되었습니다.";
        this->orderManager->orderListJsonSave();
        this->orderManager->orderListJsonLoad();
    }
    QJsonArray responseArray;
    responseArray.append(responseObj);

    CommuInfo responseInfo;
    if(type == ProductInfo::ProductType::Book){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Book, filter, QString("OrderAdd"));
    } else if (type == ProductInfo::ProductType::Music){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Music, filter, QString("OrderAdd"));
    } else if (type == ProductInfo::ProductType::Blueray){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Blueray, filter, QString("OrderAdd"));
    }
    responseInfo.AppendResponseArray(responseArray);

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)responseInfo.GetByteArray().size(); // 길이 프리픽스
    packet.append(responseInfo.GetByteArray());

    emit writeReady(client->thread, packet);
    // socket->write(packet); // 길이  + 나머지 모든 데이터(response 데이터 포함)
    // socket->flush(); // 추가적으로 송신 버퍼를 즉시 밀어줌
    qDebug() << "서버: SearchAddRespond 응답 전송 완료";
    qDebug() << responseInfo.GetByteArray();
}


void Server::DeleteOrderDataResponse(const CommuInfo& commuInfo, ClientData* client) {
    ProductInfo::Filter filter;
    ProductInfo::ProductType type = commuInfo.GetRequestProducts(filter);
    QString uuid = filter.keyword;

    // 유저 정보 찾기
    UserInfo* user = this->userManager->userSearchById(client->ID);
    if (!user) {
        qDebug() << "서버: 유저를 찾을 수 없습니다.";
        return;
    }

    // 전체 상품 목록에서 해당 UUID를 가진 상품 찾기
    ProductInfo* product = nullptr;
    if (type == ProductInfo::ProductType::Book) {
        product = this->bookManager->bookSearchByUuid(uuid);
    } else if (type == ProductInfo::ProductType::Music) {
        product = this->musicManager->musicSearchByUuid(uuid);
    } else if (type == ProductInfo::ProductType::Blueray) {
        product = this->bluerayManager->blueraySearchByUuid(uuid);
    }
    if (!product) {
        qDebug() << "서버: 해당 uuid 상품 없음";
        return;
    }
    qDebug() << "server : orderList delete orderProduct -> " << product->getName();

    // 주문 목록에 추가
    int resultDelete = this->orderManager->delOrderList(user, product);
    // 클라이언트에게 주문 응답
    QJsonObject responseObj;
    responseObj["uuid"] = uuid;
    responseObj["userID"] = user->getID();
    if(resultDelete == -1){
        qDebug() << "user 또는 product 없음";
        responseObj["status"] = "fail";
        responseObj["message"] = "user 또는 product 없음";
    } else if(resultDelete == 0){
        qDebug() << "서버: 주문 삭제가 성공적으로 완료되었습니다. UUID:" << uuid << "유저:" << user->getID();
        responseObj["status"] = "success";
        responseObj["message"] = "주문 삭제가 완료되었습니다.";
        this->orderManager->orderListJsonSave();
        this->orderManager->orderListJsonLoad();
    }
    QJsonArray responseArray;
    responseArray.append(responseObj);

    CommuInfo responseInfo;
    if(type == ProductInfo::ProductType::Book){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Book, filter, QString("OrderDelete"));
    } else if (type == ProductInfo::ProductType::Music){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Music, filter, QString("OrderDelete"));
    } else if (type == ProductInfo::ProductType::Blueray){
        responseInfo.RequestOrderProducts(ProductInfo::ProductType::Blueray, filter, QString("OrderDelete"));
    }
    responseInfo.AppendResponseArray(responseArray);

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)responseInfo.GetByteArray().size(); // 길이 프리픽스
    packet.append(responseInfo.GetByteArray());

    emit writeReady(client->thread, packet);
    // socket->write(packet); // 길이  + 나머지 모든 데이터(response 데이터 포함)
    // socket->flush(); // 추가적으로 송신 버퍼를 즉시 밀어줌
    qDebug() << "서버: SearchDeleteRespond 응답 전송 완료";
    qDebug() << responseInfo.GetByteArray();
}
