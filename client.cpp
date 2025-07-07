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


Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    this->bookmanager = this->bookmanager->getInstance();
    this->blueraymanager = this->blueraymanager->getInstance();
    this->musicmanager = this->musicmanager->getInstance();

    // 클라이언트로 접근하고, 도서 탭 리스트 뷰 구성
    this->printBookList();
    this->printBluerayList();
    this->printMusicList();
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

