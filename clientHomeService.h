#ifndef CLIENTHOMESERVICE_H
#define CLIENTHOMESERVICE_H

#include "clientBluerayService.h"
#include "clientBookService.h"
#include "clientMusicService.h"
#include "clientService.h"

class Client;

class ClientHomeService : public ClientService {
public:
    ClientHomeService();
    ~ClientHomeService();
    // 전체 product 검색 로직
    void allSearch(Client* homeTab);
    // 주문 조회 클릭 시 발생 로직
    void orderChecking(Client* homeTab);

private:
    ClientBookService* clientBookService;
    ClientBluerayService* clientBluerayService;
    ClientMusicService* clientMusicService;

};

#endif // CLIENTHOMESERVICE_H
