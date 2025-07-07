#include "clientHomeService.h"
#include "popup.h"
#include "ui_client.h"
#include "client.h"
#include "orderitem.h"
using namespace std;

ClientHomeService::ClientHomeService(){
    clientBookService = new ClientBookService();
    clientBluerayService = new ClientBluerayService();
    clientMusicService = new ClientMusicService();
}
ClientHomeService::~ClientHomeService(){}

// 통합 검색 로직
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
        // finded "book" 찾음
        QVector<Book*> searchBookResult = clientBookService->bookHomeSearch(searchData);
        homeTab->printSearchBookList(searchBookResult);
        if(!searchBookResult.isEmpty()){
            homeTab->getUi()->tabWidget->setCurrentIndex(1);
        }
    }
    // 2-2. music search
    it = find(searchType.begin(), searchType.end(), "music");
    if(it != searchType.end()){
        // finded "music" 찾음
        QVector<Music*> searchMusicResult = clientMusicService->musicHomeSearch(searchData);
        homeTab->printSearchMusicList(searchMusicResult);
        if(!searchMusicResult.isEmpty()){
            homeTab->getUi()->tabWidget->setCurrentIndex(2);
        }
    }
    // 2-3. blueray search
    it = find(searchType.begin(), searchType.end(), "blueray");
    if(it != searchType.end()){
        // finded "blueray" 찾음
        QVector<Blueray*> searchBluerayResult = clientBluerayService->bluerayHomeSearch(searchData);
        homeTab->printSearchBluerayList(searchBluerayResult);
        if(!searchBluerayResult.isEmpty()){
            homeTab->getUi()->tabWidget->setCurrentIndex(3);
        }
    }
}

void ClientHomeService::orderChecking(Client* homeTab){
    if(homeTab->getUi()->home_order_listWidget->selectedItems().isEmpty()){
        Popup* popup = new Popup(homeTab, QObject::tr("선택한 항목이 없습니다!"));
        popup->show();
    } else {
        /*
        // home 탭에서 선택된(selected) orderItem 을 감싸고 있는 틀인 QListWidgetItem 을 먼저 가져옴.
        QListWidgetItem* firstItem = homeTab->getUi()->home_order_listWidget->selectedItems().first();
        // QListWidgetItem 에 있는 QWidget 아이템 포인터 가져옴
        QWidget* widget = homeTab->getUi()->home_order_listWidget->itemWidget(firstItem);
        // qwidget 을 OrderItem* 으로 캐스팅(qobject_cast 방식으로 반환 체크를 먼저 진행하기 때문에 widget 이 OrderItem* 이 아닐 경우 nullptr 을 반환)
        OrderItem* castedItem = qobject_cast<OrderItem*>(widget);
        if(castedItem){
            QMap<QString, QString> selectedData = castedItem->getData();
            // 선택된 데이터 확인
            qDebug() << selectedData;
            Popup* popup = new Popup(homeTab, QObject::tr("조회가 완료되었습니다."));
            popup->show();
        } else {
            qDebug() << "선택한 ListwidgetItem 에 qwidget 아이템이 없거나, qwidget 캐스팅 오류가 발생하였습니다.";
        }
        */
    }
}
