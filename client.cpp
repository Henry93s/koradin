#include "client.h"
#include "ui_client.h"
#include "bookitem.h"

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    this->bookmanager = this->bookmanager->getInstance();

    // 첫 클라이언트로 접근하고, 도서 탭 리스트 뷰 구성
    this->printBookList();
}

Client::~Client()
{
    delete ui;
}

void Client::printBookList(){
    for(auto i = bookmanager->bookListRead().begin(); i != bookmanager->bookListRead().end(); i++){
        BookItem* bookitem = new BookItem(this);
        bookitem->setData(i.value());
    }
}
