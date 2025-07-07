#include "client.h"
#include "ui_client.h"
#include "bookitem.h"
#include "book.h"
#include "blueray.h"
#include "bluerayitem.h"
#include "music.h"
#include "musicitem.h"
#include <QMap>
#include <QVector>
#include <QDir>


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

    // 시작 tab 고정
    ui->tabWidget->setCurrentIndex(0);
}

Client::~Client()
{
    delete ui;
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

void Client::printSearchBookList(const QVector<Book*>& list){
    // 현재 listWidget 을 비우고 추가하여야 함
    ui->book_listWidget->clear();

    for(auto i = list.begin(); i != list.end(); i++){
        BookItem* bookitem = new BookItem(this);
        bookitem->setData(*i);
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


void Client::on_home_search_pushButton_clicked()
{
    clientHomeService.allSearch(this);
}

void Client::on_home_orderSearch_pushButton_clicked()
{
    clientHomeService.orderChecking(this);
}

