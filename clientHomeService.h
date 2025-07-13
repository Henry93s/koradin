#ifndef CLIENTHOMESERVICE_H
#define CLIENTHOMESERVICE_H

#include "clientBluerayService.h"
#include "clientBookService.h"
#include "clientMusicService.h"
#include "clientService.h"
#include "commuInfo.h"

class Client;

class ClientHomeService : public ClientService {
public:
    ClientHomeService();
    ~ClientHomeService();
    // 전체 product 검색 로직
    void allSearch(Client* homeTab);

    // client home 에서 server 에 전체 검색 요청
    void homeBookSearchRequest(Client* homeTab, const QString& searchData);
    void homeMusicSearchRequest(Client* homeTab, const QString& searchData);
    void homeBlueraySearchRequest(Client* homeTab, const QString& searchData);

    // 주문 조회 클릭 시 발생 로직
    void orderChecking(Client* homeTab);

private:
    ClientBookService* clientBookService;
    ClientBluerayService* clientBluerayService;
    ClientMusicService* clientMusicService;
};

#endif // CLIENTHOMESERVICE_H
