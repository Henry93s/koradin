#include "client.h"
#include "ui_client.h"
#include "tcpcommudefines.h"
#include "chattingroom.h"
#include "commuInfo.h"

#include <QLineEdit>
#include "bookitem.h"
#include "book.h"
#include <QMap>
#include <QVector>
#include <QMessageBox>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    // qDebug("Client");
    // //채팅 받기
    // connect(this, &Client::ChattingRespond, ui->chattingTab->GetChattingList(), [=](const CommuInfo& commuInfo){
    //     auto data = commuInfo.GetChat();
    //     ui->chattingTab->GetChattingList()->addItem(data.first + QString(" : ") + data.second);
    // });
    this->bookmanager = this->bookmanager->getInstance();

    // 첫 클라이언트로 접근하고, 도서 탭 리스트 뷰 구성
    this->printBookList();
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

    connect(clientData.socket, SIGNAL(readyRead()), SLOT(respond()));
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
    case CommuType::AUTH:

        break;
    default:
        break;
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

