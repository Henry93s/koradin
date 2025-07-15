#ifndef CLIENTMUSICSERVICE_H
#define CLIENTMUSICSERVICE_H

#include "clientService.h"
#include "musicManager.h"

class Client;

class ClientMusicService : ClientService {
public:
    ClientMusicService();
    ~ClientMusicService();

    // music 탭에서 음반 검색 을 서버에 요청
    void musicSearch(Client* musicTab);
    void musicSearchRequest(Client* musicTab, const QString& searchData, const QString& searchType, const int& beforePriceForSearch, const int& afterPriceForSearch);

    // 주문하기 클릭 시 발생 로직
    void musicOrdering(Client* musicTab);

private:
    MusicManager* musicmanager;
};

#endif // CLIENTMUSICSERVICE_H
