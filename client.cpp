#include "client.h"
#include "addAtUserList.h"
#include "ui_client.h"
#include "tcpcommudefines.h"
#include "chattingroom.h"
#include "commuInfo.h"
#include "whisperdialog.h"
#include "chattinglogwidget.h"

#include <QLineEdit>
#include "bookitem.h"
#include "book.h"
#include "blueray.h"
#include "bluerayitem.h"
#include "music.h"
#include "musicitem.h"
#include <QMap>
#include <QVector>
#include <QMessageBox>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include "orderitem.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    // 클라이언트로 접근하고, 도서 탭 리스트 뷰 구성
    this->bookmanager = BookManager::getInstance();
    this->blueraymanager = BluerayManager::getInstance();
    this->musicmanager = MusicManager::getInstance();
    this->ordermanager = OrderManager::getInstance();

    // logo 이미지 삽입
    // 이미지 라벨 사이즈 고정
    ui->home_image_label->setFixedSize(120, 120);
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString logoPath;
#ifdef Q_OS_WIN
    logoPath = QDir(runFilePath).filePath("../../../logo_image/logo.PNG");
#elif defined(Q_OS_MAC)
    logoPath = QDir(runFilePath).filePath("../../../../../logo_image/logo.PNG");
#endif
    qDebug() << "logo path : " << logoPath;
    QPixmap qimage(logoPath);
    ui->home_image_label->setPixmap(qimage);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->chatLineEdit, &QLineEdit::returnPressed, this, &Client::chatForServer);
    connect(ui->client_send_pushButton, &QPushButton::clicked, this, &Client::chatForServer);

    connect(ui->userList, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item){
        WhisperDialog* w = new WhisperDialog(item->text(), this, this);
        w->show();
    });

    // 클릭 시 파일 로드하는 버튼 연결
    connect(ui->fileOpenButton, &QPushButton::clicked, [this](){
        if(currentFileInChat){
            currentFileInChat->close();
        }
        QString filename = QFileDialog::getOpenFileName(this);
        if(filename.isEmpty())
            return;
        QFile* file = new QFile(filename);
        auto ret = file->open(QFile::ReadOnly);
        if(ret){
            currentFileInChat = file;
            QFileInfo fileInfo(filename);
            ui->fileOpenButton->setText(fileInfo.fileName());
        }
    });
}

Client::~Client()
{
    delete ui;
    socket->close();
    if(currentFileInChat){
        currentFileInChat->close();
    }
    for(auto file : filesDownloaded){
        file->close();
    }
}

void Client::Initialize(QTcpSocket *sock, const QString& Name, const QString& ID, const std::vector<QString>& otherNames)
{
    //qDebug("Client Initialize");
    //sprintf(buf, "%p, %s", sock, Name.toString().data());
    socket = sock;
    clientData.name = Name;
    clientData.ID = ID;

    qDebug() << "testings : client::initialize - name:  " << clientData.name << ", ID :  " << clientData.ID;

    // 데이터 스트림 초기화
    // in.setDevice(sock);
    // in.setVersion(QDataStream::Qt_6_0);

    //커넥트
    connect(socket, SIGNAL(readyRead()), SLOT(respond()));

    // 타 유저리스트 초기화
    for(auto name : otherNames){
        if(name == tr("-")){
            continue;
        }
        ui->userList->addItem(name);
    }

    // 첫 실행 시 모든 product 검색 처리(단 tab index 는 넘어가지 않아야 함)
    this->on_home_orderSearch_pushButton_clicked();
}

void Client::respond()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    // base64 이미지 대용량 byte 가 있는 경우가 있으므로 소켓에서 길이를 먼저 읽어내고
    // 그 길이만큼 전부 도착할 때까지 read를 반복한 다음에 파싱 작업 들어가야함

    QDataStream in(socket);
    // in.setVersion(QDataStream::Qt_6_0);
    qDebug() << "Response!!!";
     qDebug() << "ExpectedSize :" << expectedSize;
     qDebug() << "ClinetSocket - bytesAvailable :" << clientSocket->bytesAvailable();
    while(1){
        if(expectedSize == 0){
            if(clientSocket->bytesAvailable() < 4){
                qDebug() << "Break. clientSocket - bytesAvailable :" << clientSocket->bytesAvailable() << " Expected Size :" << expectedSize;
                break; // 길이 정보가 아직 다 들어오지 않음
            }
            in >> expectedSize;
        }

        if(clientSocket->bytesAvailable() < expectedSize){
            qDebug() << "Break2. clientSocket - bytesAvailable :" << clientSocket->bytesAvailable() << " Expected Size :" << expectedSize;
            break; // 아직 데이터가 다 들어오지 않음
        }

        buffer = clientSocket->read(expectedSize);

        auto info = CommuInfo{buffer};
        auto type = info.GetType();

        qDebug() << "respond type : " << type;

        switch(type){
        case CommuType::Infos:
            InfosFetchRespond(info);
            break;
        case CommuType::Chatting:
            ChattingRespond(info);
            break;
        case CommuType::InfoFix:
            emit InfosFixRespond(info);
            break;
        case CommuType::AUTH:

            break;
        case CommuType::LOGINOUT:
            SomeoneLoginOrOutRespond(info);
        case CommuType::OrderInfos:
            OrderInfosFetchRespond(info);
            break;
        case CommuType::OrderAdd:
            OrderAddRespond(info);
            break;
        case CommuType::OrderDelete:
            OrderAddRespond(info);
            break;
        default:
            break;
        }
        expectedSize = 0; // 이번 패킷 처리가 끝났으므로 다음 패킷 수신을 위해 기대하는 길이를 0으로 초기화
        buffer.clear(); // 이전 패킷에서 받은 데이터는 처리 완료되었으므로 버퍼를 비워 다음 수신에 대비
    }
}

void Client::InfosFetchRespond(const CommuInfo &commuInfo)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);

    qDebug() << "client : producttype : " << productType;

    switch (productType) {
    case ProductInfo::Book:
        printBookSearchData(commuInfo);
        break;
    case ProductInfo::Blueray:
        printBlueraySearchData(commuInfo);
        break;
    case ProductInfo::Music:
        printMusicSearchData(commuInfo);
        break;
    default:
        break;
    }

}

void Client::OrderInfosFetchRespond(const CommuInfo &commuInfo)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);

    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        printOrderSearchData(commuInfo);
        break;
    default:
        break;
    }

}

void Client::OrderAddRespond(const CommuInfo &commuInfo)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);

    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        OrderAddResponse(commuInfo);
        break;
    default:
        break;
    }
}

void Client::OrderDeleteRespond(const CommuInfo &commuInfo)
{
    ProductInfo::Filter filter;
    ProductInfo::ProductType productType = commuInfo.GetRequestProducts(filter);

    switch (productType) {
    case ProductInfo::Book:
    case ProductInfo::Blueray:
    case ProductInfo::Music:
        OrderDeleteResponse(commuInfo);
        break;
    default:
        break;
    }
}

void Client::OrderAddResponse(const CommuInfo &commuInfo) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);

    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, tr("파싱 오류"), error.errorString());
        return;
    }

    if (!doc.isObject()) {
        QMessageBox::critical(this, tr("응답 오류"), tr("서버 응답 형식이 잘못되었습니다."));
        return;
    }

    QJsonObject root = doc.object();

    if (!root.contains("response") || !root["response"].isArray()) {
        QMessageBox::critical(this, tr("응답 오류"), tr("서버 응답에 필요한 데이터가 없습니다."));
        return;
    }

    QJsonArray responseArray = root["response"].toArray();
    if (responseArray.isEmpty()) {
        QMessageBox::information(this, tr("응답"), tr("서버로부터 결과가 도착하지 않았습니다."));
        return;
    }

    QJsonObject obj = responseArray.first().toObject();

    QString status = obj.value("status").toString();
    QString message = obj.value("message").toString();
    QString uuid = obj.value("uuid").toString();
    QString userID = obj.value("userID").toString();

    QString title = (status == "success") ? tr("주문 완료") : tr("주문 실패");

    QMessageBox::information(this, title, message);
    clientHomeService.orderChecking(this);
}

void Client::OrderDeleteResponse(const CommuInfo &commuInfo) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);

    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, tr("파싱 오류"), error.errorString());
        return;
    }

    if (!doc.isObject()) {
        QMessageBox::critical(this, tr("응답 오류"), tr("서버 응답 형식이 잘못되었습니다."));
        return;
    }

    QJsonObject root = doc.object();

    if (!root.contains("response") || !root["response"].isArray()) {
        QMessageBox::critical(this, tr("응답 오류"), tr("서버 응답에 필요한 데이터가 없습니다."));
        return;
    }

    QJsonArray responseArray = root["response"].toArray();
    if (responseArray.isEmpty()) {
        QMessageBox::information(this, tr("응답"), tr("서버로부터 결과가 도착하지 않았습니다."));
        return;
    }

    QJsonObject obj = responseArray.first().toObject();

    QString status = obj.value("status").toString();
    QString message = obj.value("message").toString();
    QString uuid = obj.value("uuid").toString();
    QString userID = obj.value("userID").toString();

    QString title = (status == "success") ? tr("주문 삭제 완료") : tr("주문 삭제 실패");

    QMessageBox::information(this, title, message);
    clientHomeService.orderChecking(this);
}


void Client::printOrderSearchData(const CommuInfo& commuInfo){
    ui->home_order_listWidget->clear();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "파싱 오류", error.errorString());
        return;
    }

    QJsonObject rootObj = doc.object();
    QString productTypes = rootObj["Info"].toObject()["Product"].toObject()["ProductType"].toString();

    // response 배열에서 첫 번째
    QJsonArray responseArr = rootObj["response"].toArray();
    if (responseArr.isEmpty()) {
        qDebug() << "response 비어있음";
        return;
    }
    QJsonObject responseObj = responseArr.at(0).toObject();

    // orderItems의 uuid 리스트 파싱
    QJsonArray orderItemsArr = responseObj["orderItems"].toArray();
    qDebug() << "orderItemsArr " << orderItemsArr;
    for (auto val : orderItemsArr) {
        QString uuid = val.toObject()["uuid"].toString();
        QString name;
        int price;

        qDebug() << "orderItemsArr uuid : " << uuid;

        if(productTypes.compare("Book") == 0){
            Book* book = this->bookmanager->bookSearchByUuid(uuid);
            if (!book) {
                qDebug() << "skip not type uuid " << uuid;
                continue;
            }
            name = book->getName();
            price = book->getPrice();
            qDebug() << name << " " << price;
        } else if(productTypes.compare("Music") == 0){
            Music* music = this->musicmanager->musicSearchByUuid(uuid);
            if (!music) {
                qDebug() << "skip not type uuid " << uuid;
                continue;
            }
            name = music->getName();
            price = music->getPrice();
        } else { //if(productType.compare("blueray") == 0){
            Blueray* blueray = this->blueraymanager->blueraySearchByUuid(uuid);
            if (!blueray) {
                qDebug() << "skip not type uuid " << uuid;
                continue;
            }
            name = blueray->getName();
            price = blueray->getPrice();
        }

        OrderItem* orderItem = new OrderItem(this);
        orderItem->setData(productTypes, name, price, uuid);

        QListWidgetItem* item = new QListWidgetItem(ui->home_order_listWidget);
        item->setSizeHint(orderItem->sizeHint());
        ui->home_order_listWidget->addItem(item);
        ui->home_order_listWidget->setItemWidget(item, orderItem);
    }
}

void Client::printBookSearchData(const CommuInfo& commuInfo) {
    ui->book_listWidget->clear();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "파싱 오류", error.errorString());
        return;
    }

    QJsonArray results = doc.object()["response"].toArray();
    for (const QJsonValue& val : results) {
        QJsonObject obj = val.toObject();
        Book* book = Book::fromJsonObject(obj);

        BookItem* bookItem = new BookItem(this);
        bookItem->setData(book);

        QListWidgetItem* item = new QListWidgetItem(ui->book_listWidget);
        item->setSizeHint(bookItem->sizeHint());
        ui->book_listWidget->addItem(item);
        ui->book_listWidget->setItemWidget(item, bookItem);
    }
    if(is_first_search < 3){
        is_first_search += 1;
    } else if(is_first_search >= 3 && !results.empty()){
        ui->tabWidget->setCurrentIndex(1);
    }
}

void Client::printMusicSearchData(const CommuInfo& commuInfo) {
    ui->music_listWidget->clear();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "파싱 오류", error.errorString());
        return;
    }

    QJsonArray results = doc.object()["response"].toArray();
    for (const QJsonValue& val : results) {
        QJsonObject obj = val.toObject();
        Music* music = Music::fromJsonObject(obj);

        MusicItem* musicItem = new MusicItem(this);
        musicItem->setData(music);

        QListWidgetItem* item = new QListWidgetItem(ui->music_listWidget);
        item->setSizeHint(musicItem->sizeHint());
        ui->music_listWidget->addItem(item);
        ui->music_listWidget->setItemWidget(item, musicItem);
    }
    if(is_first_search < 3){
        is_first_search += 1;
    } else if(is_first_search >= 3 && !results.empty()){
        ui->tabWidget->setCurrentIndex(2);
    }
}


void Client::printBlueraySearchData(const CommuInfo& commuInfo) {
    ui->blueray_listWidget->clear();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(commuInfo.GetByteArray(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "파싱 오류", error.errorString());
        return;
    }

    QJsonArray results = doc.object()["response"].toArray();
    for (const QJsonValue& val : results) {
        QJsonObject obj = val.toObject();
        Blueray* blueray = Blueray::fromJsonObject(obj);

        BluerayItem* bluerayItem = new BluerayItem(this);
        bluerayItem->setData(blueray);

        QListWidgetItem* item = new QListWidgetItem(ui->blueray_listWidget);
        item->setSizeHint(bluerayItem->sizeHint());
        ui->blueray_listWidget->addItem(item);
        ui->blueray_listWidget->setItemWidget(item, bluerayItem);
    }
    if(is_first_search < 3){
        is_first_search += 1;
    } else if(is_first_search >= 3 && !results.empty()){
        ui->tabWidget->setCurrentIndex(3);
    }
}

void Client::SomeoneLoginOrOutRespond(const CommuInfo &commuInfo)
{
    qDebug() << "sent client:" << clientData.name;
    bool isLogin;
    auto userNames = commuInfo.GetConfirmLoginOrOut(isLogin);

    ui->userList->clear();
    for(auto& Name : userNames){
        if(Name == tr("-")){
            continue;
        }
        ui->userList->addItem(Name);
    }
}

void Client::ChattingRespond(const CommuInfo &commuInfo)
{
    ChattingType type; QString n; QByteArray fileBytes;
    QString fileName;
    auto nameAndChat = commuInfo.GetChat(type, n, fileBytes, fileName);

    // 채팅 리스트에 추가.
    AddAtChattingList(this, ui->client_chatting_listWidget, nameAndChat.first, nameAndChat.second, fileBytes, fileName);
}

void Client::chatForServer()
{
    QString chat = ui->chatLineEdit->text();
    CommuInfo com;
    com.SetChat(clientData.name, chat, currentFileInChat);
    // 김선권 추가
    com.AddSizePacket();
    socket->write(com.GetByteArray());
    if(currentFileInChat){
        currentFileInChat->close();
        delete currentFileInChat;
        currentFileInChat = nullptr;
        ui->fileOpenButton->setText(tr("File"));
    }
}

Ui::Client* Client::getUi(){
    return this->ui;
}

ClientData* Client::getClientData(){
    return &(this->clientData);
}

// book 탭에서 도서 검색
void Client::on_book_search_pushButton_clicked()
{
    clientBookService.bookSearch(this);
}

void Client::on_book_order_pushButton_clicked()
{
    clientBookService.bookOrdering(this);
}

// music 탭에서 음반 검색
void Client::on_music_search_pushButton_clicked()
{
    clientMusicService.musicSearch(this);
}

void Client::on_music_order_pushButton_clicked()
{
    clientMusicService.musicOrdering(this);
}

// blueray 탭에서 블루레이 검색
void Client::on_blueray_search_pushButton_clicked()
{
    clientBluerayService.blueraySearch(this);
}

void Client::on_blueray_order_pushButton_clicked()
{
    clientBluerayService.bluerayOrdering(this);
}


// home 탭 전체 검색 동작
void Client::on_home_search_pushButton_clicked()
{
    clientHomeService.allSearch(this);
}

void Client::on_home_orderSearch_pushButton_clicked()
{
    clientHomeService.orderChecking(this);
}

void Client::closeEvent(QCloseEvent *event)
{
    CommuInfo com;
    qDebug() << "client::closeevent = name : " << clientData.name << " id : " << clientData.ID;
    com.LoginOrOut(false, clientData.name, clientData.ID);
    // 김선권 추가
    com.AddSizePacket();

    socket->write(com.GetByteArray());

    QWidget::closeEvent(event);
}

void Client::on_home_orderDelete_pushButton_clicked()
{
    clientHomeService.orderDelete(this);
}


