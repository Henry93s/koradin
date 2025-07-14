#ifndef CLIENTBLUERAYSERVICE_H
#define CLIENTBLUERAYSERVICE_H

#include "clientService.h"
#include "bluerayManager.h"

class Client;

class ClientBluerayService : ClientService {
public:
    ClientBluerayService();
    ~ClientBluerayService();
    // blueray 탭에서 블루레이 검색 을 서버에 요청
    void blueraySearch(Client* bluerayTab);
    void blueraySearchRequest(Client* bluerayTab, const QString& searchData, const QString& searchType, const int& beforePriceForSearch, const int& afterPriceForSearch);

    // 주문하기 클릭 시 발생 로직
    void bluerayOrdering(Client* bluerayTab);

private:
    BluerayManager* blueraymanager;
};

#endif // CLIENTBLUERAYSERVICE_H
