#include "clientHomeService.h"
#include "popup.h"
#include "ui_client.h"
#include "client.h"
#include "orderitem.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

using namespace std;

ClientHomeService::ClientHomeService(){
    clientBookService = new ClientBookService();
    clientBluerayService = new ClientBluerayService();
    clientMusicService = new ClientMusicService();
}
ClientHomeService::~ClientHomeService(){
    delete clientBookService;
    delete clientBluerayService;
    delete clientMusicService;
}

// 1. 통합 검색 로직
void ClientHomeService::allSearch(Client* homeTab){
    // 1. home tab 에서 checkbox button 에 따른 검색 조건 설정
    QVector<QString> searchType; // empty : 아무 체크 박스도 선택 안 함.
    if(homeTab->getUi()->home_search_book_checkbox->isChecked()){
        searchType.append("book");
    }
    if(homeTab->getUi()->home_search_music_checkbox->isChecked()){
        searchType.append("music");
    }
    if(homeTab->getUi()->home_search_blueray_checkbox->isChecked()){
        searchType.append("blueray");
    }

    // 2. 검색어 입력 체크(입력하지 않았을 때 전체 데이터를 출력)
    QString searchData = homeTab->getUi()->home_search_lineEdit->text();
    // 2-1. book search
    auto it = find(searchType.begin(), searchType.end(), "book");
    if(it != searchType.end()){
        // homeBookSearchRequest 메소드로 서버에 book 검색 요청 처리
        QMessageBox::information(homeTab, "Book Search", "도서 정보를 불러오고 있습니다.");
        this->homeBookSearchRequest(homeTab, searchData);
    }

    // 2-2. music search
    it = find(searchType.begin(), searchType.end(), "music");
    if(it != searchType.end()){
        // homeMusicSearchRequest 메소드로 서버에 music 검색 요청 처리
        QMessageBox::information(homeTab, "Music Search", "음반 정보를 불러오고 있습니다.");
        this->homeMusicSearchRequest(homeTab, searchData);
    }

    // 2-3. blueray search
    it = find(searchType.begin(), searchType.end(), "blueray");
    if(it != searchType.end()){
        // homeBlueraySearchRequest 메소드로 서버에 blueray 검색 요청 처리
        QMessageBox::information(homeTab, "Blueray Search", "블루레이 정보를 불러오고 있습니다.");
        this->homeBlueraySearchRequest(homeTab, searchData);
    }
}

// 2. 클라이언트 HOME 에서 책 검색 을 서버에 요청
void ClientHomeService::homeBookSearchRequest(Client* homeTab, const QString& searchData){
    // 검색 데이터 json 작성 및 서버에 요청
    CommuInfo commuinfo;
    commuinfo.RequestProducts(ProductInfo::ProductType::Book
                              , ProductInfo::Filter{ ProductInfo::FilterType::Name, searchData, 0, 9999999});

    homeTab->writeSocket(commuinfo.GetByteArray());
}

// 2. 클라이언트 HOME 에서 음반 검색 을 서버에 요청
void ClientHomeService::homeMusicSearchRequest(Client* homeTab, const QString& searchData){
    // 검색 데이터 json 작성 및 서버에 요청
    CommuInfo commuinfo;
    commuinfo.RequestProducts(ProductInfo::ProductType::Music
                              , ProductInfo::Filter{ ProductInfo::FilterType::Name, searchData, 0, 9999999});

    homeTab->writeSocket(commuinfo.GetByteArray());
}

// 2. 클라이언트 HOME 에서 블루레이 검색 을 서버에 요청
void ClientHomeService::homeBlueraySearchRequest(Client* homeTab, const QString& searchData){
    // 검색 데이터 json 작성 및 서버에 요청
    CommuInfo commuinfo;
    commuinfo.RequestProducts(ProductInfo::ProductType::Blueray
                              , ProductInfo::Filter{ ProductInfo::FilterType::Name, searchData, 0, 9999999});

    homeTab->writeSocket(commuinfo.GetByteArray());
}


void ClientHomeService::orderChecking(Client* homeTab){
    CommuInfo commuinfo;
    ProductInfo::ProductType productType = ProductInfo::ProductType::None;

    if(homeTab->getUi()->home_order_blueray_radioButton->isChecked()){
        productType = ProductInfo::ProductType::Blueray;
    } else if(homeTab->getUi()->home_order_book_radioButton->isChecked()){
        productType = ProductInfo::ProductType::Book;
    } else if(homeTab->getUi()->home_order_music_radioButton->isChecked()){
        productType = ProductInfo::ProductType::Music;
    }

    QString searchData = homeTab->getUi()->home_orderSearchlineEdit->text();
    commuinfo.RequestOrderProducts(productType, ProductInfo::Filter{ProductInfo::FilterType::Name, searchData, 0, 9999999});

    homeTab->writeSocket(commuinfo.GetByteArray());
}
