#ifndef CLIENTMUSICSERVICE_H
#define CLIENTMUSICSERVICE_H

#include "clientService.h"
#include "musicManager.h"

class Client;

class ClientMusicService : ClientService {
public:
    ClientMusicService();
    ~ClientMusicService();

    // 다중 검색 로직
    QVector<Music*> musicSearch(Client* musicTab);
    // 주문하기 클릭 시 발생 로직
    void musicOrdering(Client* musicTab);

private:
    MusicManager* musicmanager;
};

#endif // CLIENTMUSICSERVICE_H
