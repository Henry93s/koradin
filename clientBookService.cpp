#include "clientBookService.h"
#include "popup.h"
#include "ui_client.h"
#include "client.h"
#include "bookitem.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QMessageBox>


ClientBookService::ClientBookService(){
    // bookmanager singleton 인스턴스 get
    this->bookmanager = this->bookmanager->getInstance();

}
ClientBookService::~ClientBookService(){

}

// 1. 책 다중 검색
void ClientBookService::bookSearch(Client* bookTab){
    // 1. book tab 에서 radio button 에 따른 검색 조건 설정
    QString searchType =
        bookTab->getUi()->book_name_radioButton->isChecked() == true ? "name" :
        bookTab->getUi()->book_publisher_radioButton->isChecked() == true ? "company" :
        // 책 이름 / 출판사 / 작가 3 종류의 검색 조건 중 하나가 지정되지 않는 버그가 발생할 때를 방지한 예외 처리 "none"
        bookTab->getUi()->book_writer_radioButton->isChecked() == true ? "writer" : "none";

    // 2. 가격 범위 체크(두 번째 가격 범위 설정을 입력하지 않았거나 잘못된 문자열이 있을 경우 0 ~ 9999999 로 검색)
    int beforePriceForSearch = bookTab->getUi()->book_price_before_lineEdit->text().toInt();
    int afterPriceForSearch = bookTab->getUi()->book_price_after_lineEdit->text().toInt();
    // toInt() 함수에서 문자열이 정수가 아니면 0 이 반환되므로 이를 기반으로 예외처리 if
    if(afterPriceForSearch == 0){
        afterPriceForSearch = 9999999;
    }

    // 3. 검색어 입력 체크(입력하지 않았을 때 전체 데이터를 출력)
    QString searchData = bookTab->getUi()->book_search_lineEdit->text();
    QMessageBox::information(bookTab, "도서 검색", "도서 검색을 진행합니다.");
    this->bookSearchRequest(bookTab, searchData, searchType, beforePriceForSearch, afterPriceForSearch);
}

// 2. bookTab 에서 book 검색을 서버에 요청한다.
void ClientBookService::bookSearchRequest(Client* bookTab, const QString& searchData, const QString& searchType, const int& beforePriceForSearch, const int& afterPriceForSearch){
    // 통신 데이터 json 구성 및 서버에 요청
    CommuInfo commuinfo;
    if(searchType.compare("name") == 0){
        commuinfo.RequestProducts(ProductInfo::ProductType::Book \
                                  , ProductInfo::Filter \
                                  {ProductInfo::FilterType::Name, searchData, \
                                   beforePriceForSearch, afterPriceForSearch});

    } else if (searchType.compare("writer") == 0){
        commuinfo.RequestProducts(ProductInfo::ProductType::Book \
                                  , ProductInfo::Filter \
                                  {ProductInfo::FilterType::Author, searchData, \
                                   beforePriceForSearch, afterPriceForSearch});

    } else if (searchType.compare("company") == 0){
        commuinfo.RequestProducts(ProductInfo::ProductType::Book \
                                  , ProductInfo::Filter \
                                  {ProductInfo::FilterType::Company, searchData, \
                                   beforePriceForSearch, afterPriceForSearch});

    }

    bookTab->getClientData()->socket->write(commuinfo.GetByteArray());
}

void ClientBookService::bookOrdering(Client* bookTab){
    if(bookTab->getUi()->book_listWidget->selectedItems().isEmpty()){
        Popup* popup = new Popup(bookTab, QObject::tr("선택한 항목이 없습니다!"));
        popup->show();
    } else {
        // book 탭에서 선택된(selected) bookItem 을 감싸고 있는 틀인 QListWidgetItem 을 먼저 가져옴.
        QListWidgetItem* firstItem = bookTab->getUi()->book_listWidget->selectedItems().first();
        // QListWidgetItem 에 있는 QWidget 아이템 포인터 가져옴
        QWidget* widget = bookTab->getUi()->book_listWidget->itemWidget(firstItem);
        // qwidget 을 BookItem* 으로 캐스팅(qobject_cast 방식으로 반환 체크를 먼저 진행하기 때문에 widget 이 BookItem* 이 아닐 경우 nullptr 을 반환)
        BookItem* castedItem = qobject_cast<BookItem*>(widget);
        if(castedItem){
            Book* selectedData = castedItem->getData();
            // 선택된 데이터를 orderManager 에 전달해야 함
            qDebug() << selectedData;
            Popup* popup = new Popup(bookTab, QObject::tr("주문이 완료되었습니다."));
            popup->show();
        } else {
            qDebug() << "선택한 ListwidgetItem 에 qwidget 아이템이 없거나, qwidget 캐스팅 오류가 발생하였습니다.";
        }
    }
}

