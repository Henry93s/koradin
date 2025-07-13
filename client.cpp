#include "client.h"
#include "ui_client.h"
#include "tcpcommudefines.h"
#include "chattingroom.h"
#include "commuInfo.h"

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


Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    // 클라이언트로 접근하고, 도서 탭 리스트 뷰 구성
    this->bookmanager = this->bookmanager->getInstance();
    this->blueraymanager = this->blueraymanager->getInstance();
    this->musicmanager = this->musicmanager->getInstance();    
    this->printBookList();
    this->printBluerayList();
    this->printMusicList();

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
}

Client::~Client()
{
    delete ui;
    clientData.socket->close();
}

void Client::Initialize(QTcpSocket *sock, const QString& Name)
{
    //qDebug("Client Initialize");
    //sprintf(buf, "%p, %s", sock, Name.toString().data());
    clientData.socket = sock;
    clientData.name = Name;

    connect(clientData.socket, SIGNAL(readyRead()), this, SLOT(respond()));

    // 첫 실행 시 모든 product 검색 처리(단 tab index 는 넘어가지 않아야 함)
    CommuInfo requestProducts_book;
    requestProducts_book.RequestProducts(ProductInfo::ProductType::Book, ProductInfo::Filter{});
    sock->write(requestProducts_book.GetByteArray());
    CommuInfo requestProducts_music;
    requestProducts_music.RequestProducts(ProductInfo::ProductType::Music, ProductInfo::Filter{});
    sock->write(requestProducts_music.GetByteArray());

}

void Client::respond()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    // if(clientSocket->bytesAvailable() > BLOCK_SIZE) return;
    // base64 이미지 대용량 byte 가 있는 경우가 있으므로 소켓에서 길이를 먼저 읽어내고
    // 그 길이만큼 전부 도착할 때까지 read를 반복한 다음에 파싱 작업 들어가야함
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_6_0);

    while(1){
        if(expectedSize == 0){
            if(clientSocket->bytesAvailable() < 4){
                break; // 길이 정보가 아직 다 들어오지 않음
            }
            in >> expectedSize;
        }

        if(clientSocket->bytesAvailable() < expectedSize){
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
            emit ChattingRespond(info);
            break;
        case CommuType::InfoFix:
            emit InfosFixRespond(info);
            break;
        case CommuType::AUTH:

            break;
        default:
            break;
        }
        expectedSize = 0; // 이번 패킷 처리가 끝났으므로 다음 패킷 수신을 위해 기대하는 길이를 0으로 초기화
        buffer.clear(); // 이전 패킷에서 받은 데이터는 처리 완료되었으므로 버퍼를 비워 다음 수신에 대비
    }
}

void Client::printBookList(){
    QMap<QString, Book*> list = bookmanager->bookListRead();

    for(auto i = list.begin(); i != list.end(); i++){
        BookItem* bookitem = new BookItem(this);
        bookitem->setData(i.value());
        // QListWidget 에 bookitem 을 각각 채워넣음
        // 한 bookitem 의 자리를 마련
        QListWidgetItem* item = new QListWidgetItem(ui->book_listWidget);
        // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
        // qDebug() << bookitem->sizeHint(); // bookitem QSize 확인
        item->setSizeHint(bookitem->sizeHint());
        // 리스트 위젯에 아이템 추가
        ui->book_listWidget->addItem(item);
        // 생성된 틀에 커스텀 위젯을 설정
        ui->book_listWidget->setItemWidget(item, bookitem);
    }
}

void Client::printMusicList(){
    QMap<QString, Music*> list = musicmanager->musicListRead();

    for(auto i = list.begin(); i != list.end(); i++){
        MusicItem* musicitem = new MusicItem(this);
        musicitem->setData(i.value());
        // QListWidget 에 musicitem 을 각각 채워넣음
        // 한 bookitem 의 자리를 마련
        QListWidgetItem* item = new QListWidgetItem(ui->music_listWidget);
        // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
        // qDebug() << musicitem->sizeHint(); // musicitem QSize 확인
        item->setSizeHint(musicitem->sizeHint());
        // 리스트 위젯에 아이템 추가
        ui->music_listWidget->addItem(item);
        // 생성된 틀에 커스텀 위젯을 설정
        ui->music_listWidget->setItemWidget(item, musicitem);
    }
}

void Client::printSearchMusicList(const QVector<Music*>& list){
    // 현재 listWidget 을 비우고 추가하여야 함
    ui->music_listWidget->clear();

    for(auto i = list.begin(); i != list.end(); i++){
        MusicItem* musicitem = new MusicItem(this);
        musicitem->setData(*i);
        QListWidgetItem* item = new QListWidgetItem(ui->music_listWidget);
        // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
        item->setSizeHint(musicitem->sizeHint());
        // 리스트 위젯에 아이템 추가
        ui->music_listWidget->addItem(item);
        // 생성된 틀에 커스텀 위젯을 설정
        ui->music_listWidget->setItemWidget(item, musicitem);
    }
}

void Client::printBluerayList(){
    QMap<QString, Blueray*> list = blueraymanager->bluerayListRead();

    for(auto i = list.begin(); i != list.end(); i++){
        BluerayItem* bluerayitem = new BluerayItem(this);
        bluerayitem->setData(i.value());
        // QListWidget 에 bluerayitem 을 각각 채워넣음
        // 한 bluerayitem 의 자리를 마련
        QListWidgetItem* item = new QListWidgetItem(ui->blueray_listWidget);
        // 틀의 크기를 커스텀 위젯의 크기를 반영하여 set
        // qDebug() << bluerayitem->sizeHint(); // bluerayitem QSize 확인
        item->setSizeHint(bluerayitem->sizeHint());
        // 리스트 위젯에 아이템 추가
        ui->blueray_listWidget->addItem(item);
        // 생성된 틀에 커스텀 위젯을 설정
        ui->blueray_listWidget->setItemWidget(item, bluerayitem);
    }
}

///


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
    if(is_first_search < 1){
        is_first_search += 1;
    } else if(is_first_search >= 1 && !results.empty()){
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
    if(is_first_search < 1){
        is_first_search += 1;
    } else if(is_first_search >= 1 && !results.empty()){
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
    if(is_first_search < 1){
        is_first_search += 1;
    } else if(is_first_search >= 1 && !results.empty()){
        ui->tabWidget->setCurrentIndex(3);
    }
}


void Client::printSearchBluerayList(const QVector<Blueray*>& list){
    // 현재 listWidget 을 비우고 추가하여야 함
    ui->blueray_listWidget->clear();

    for(auto i = list.begin(); i != list.end(); i++){
        BluerayItem* bluerayitem = new BluerayItem(this);
        bluerayitem->setData(*i);
        QListWidgetItem* item = new QListWidgetItem(ui->blueray_listWidget);
        item->setSizeHint(bluerayitem->sizeHint());
        // 리스트 위젯에 아이템 추가
        ui->blueray_listWidget->addItem(item);
        // 생성된 틀에 커스텀 위젯을 설정
        ui->blueray_listWidget->setItemWidget(item, bluerayitem);
    }
}

Ui::Client* Client::getUi(){
    return this->ui;
}

ClientData* Client::getClientData(){
    return &(this->clientData);
}

void Client::on_book_search_pushButton_clicked()
{
    clientBookService.bookSearch(this);
}

void Client::on_book_order_pushButton_clicked()
{
    clientBookService.bookOrdering(this);
}


void Client::on_music_search_pushButton_clicked()
{
    clientMusicService.musicSearch(this);
}

void Client::on_music_order_pushButton_clicked()
{
    clientMusicService.musicOrdering(this);
}


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

