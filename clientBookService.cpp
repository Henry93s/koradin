#include "clientBookService.h"

ClientBookService::ClientBookService(){
    // bookmanager singleton 인스턴스 get
    this->bookmanager = this->bookmanager->getInstance();

}
ClientBookService::~ClientBookService(){}

// 다중 검색 로직
QVector<Book*> ClientBookService::bookSearch(QWidget* bookTab){
    QVector<Book*> searchResult;
    QMap<QString, Book*> list = this->bookmanager->bookListRead();
    for(auto i : list){
        // 1. book tab 에서 radio button 에 따른 검색 조건 설정

    }
    return searchResult;
}
