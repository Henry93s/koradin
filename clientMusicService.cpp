#include "clientMusicService.h"
#include "popup.h"
#include "client.h"
#include "ui_client.h"
#include "musicitem.h"

ClientMusicService::ClientMusicService() {
    this->musicmanager = this->musicmanager->getInstance();
}

ClientMusicService::~ClientMusicService(){}

// 다중 검색 로직
QVector<Music*> ClientMusicService::musicSearch(Client* musicTab){
    // 1. music tab 에서 radio button 에 따른 검색 조건 설정
    QString searchType =
        musicTab->getUi()->music_name_radioButton->isChecked() == true ? "name" :
            musicTab->getUi()->music_company_radioButton->isChecked() == true ? "company" :
            // 음반 이름 / 제작사 / 아티스트 3 종류의 검색 조건 중 하나가 지정되지 않는 버그가 발생할 때를 방지한 예외 처리 "none"
            musicTab->getUi()->music_artist_radioButton->isChecked() == true ? "artist" : "none";

    // 2. 가격 범위 체크(두 번째 가격 범위 설정을 입력하지 않았거나 잘못된 문자열이 있을 경우 0 ~ 9999999 로 검색)
    int beforePriceForSearch = musicTab->getUi()->music_price_before_lineEdit->text().toInt();
    int afterPriceForSearch = musicTab->getUi()->music_price_after_lineEdit->text().toInt();
    // toInt() 함수에서 문자열이 정수가 아니면 0 이 반환되므로 이를 기반으로 예외처리 if
    if(afterPriceForSearch == 0){
        afterPriceForSearch = 9999999;
    }

    // 3. 검색어 입력 체크(입력하지 않았을 때 전체 데이터를 출력)
    QString searchData = musicTab->getUi()->music_search_lineEdit->text();
    QVector<Music*> searchResult; // search 결과 vector
    QMap<QString, Music*> list = this->musicmanager->musicListRead();
    if(searchType.compare("none") == 0){
        qDebug() << "검색 조건 설정을 한 개 이상 선택해주세요.";
        Popup* popup = new Popup(musicTab, QObject::tr("검색 조건 설정을 한 개 이상 선택해주세요."));
        popup->show();
    } else {
        for(auto i = list.begin(); i != list.end(); ++i){
            if(searchType.compare("name") == 0){
                // 음반 이름으로 검색
                qDebug() << "음반 이름으로 검색 진행";
                if(i.value()->getPrice() >= beforePriceForSearch && i.value()->getPrice() <= afterPriceForSearch
                    && i.value()->getName().contains(searchData) == true){
                    qDebug() << i.value()->getName();
                    searchResult.append(i.value());
                }
            } else if(searchType.compare("company") == 0){
                // 음반 제작사로 검색
                qDebug() << "음반 제작사로 검색 진행";
                if(i.value()->getPrice() >= beforePriceForSearch && i.value()->getPrice() <= afterPriceForSearch
                    && i.value()->getCompany().contains(searchData) == true){
                    qDebug() << i.value()->getName();
                    searchResult.append(i.value());
                }
            } else if(searchType.compare("artist") == 0){
                // 아티스트로 검색
                qDebug() << "아티스트로 검색 진행";
                if(i.value()->getPrice() >= beforePriceForSearch && i.value()->getPrice() <= afterPriceForSearch
                    && i.value()->getArtist().contains(searchData) == true){
                    qDebug() << i.value()->getName();
                    searchResult.append(i.value());
                }
            }
        }
        musicTab->printSearchMusicList(searchResult);
    }

    return searchResult;
}

void ClientMusicService::musicOrdering(Client* musicTab){
    if(musicTab->getUi()->music_listWidget->selectedItems().isEmpty()){
        Popup* popup = new Popup(musicTab, QObject::tr("선택한 항목이 없습니다!"));
        popup->show();
    } else {
        // music 탭에서 선택된(selected) musicItem 을 감싸고 있는 틀인 QListWidgetItem 을 먼저 가져옴.
        QListWidgetItem* firstItem = musicTab->getUi()->music_listWidget->selectedItems().first();
        // QListWidgetItem 에 있는 QWidget 아이템 포인터 가져옴
        QWidget* widget = musicTab->getUi()->music_listWidget->itemWidget(firstItem);
        // qwidget 을 MusicItem* 으로 캐스팅(qobject_cast 방식으로 반환 체크를 먼저 진행하기 때문에 widget 이 MusicItem* 이 아닐 경우 nullptr 을 반환)
        MusicItem* castedItem = qobject_cast<MusicItem*>(widget);
        if(castedItem){
            QMap<QString, QString> selectedData = castedItem->getData();
            // 선택된 데이터를 orderManager 에 전달해야 함
            qDebug() << selectedData;
            Popup* popup = new Popup(musicTab, QObject::tr("주문이 완료되었습니다."));
            popup->show();
        } else {
            qDebug() << "선택한 ListwidgetItem 에 qwidget 아이템이 없거나, qwidget 캐스팅 오류가 발생하였습니다.";
        }
    }
}

