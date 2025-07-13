#ifndef CLIENTBLUERAYSERVICE_H
#define CLIENTBLUERAYSERVICE_H

#include "clientService.h"
#include "bluerayManager.h"

class Client;

class ClientBluerayService : ClientService {
public:
    ClientBluerayService();
    ~ClientBluerayService();    // 다중 검색 로직
    QVector<Blueray*> blueraySearch(Client* bluerayTab);
    // home call -> search
    QVector<Blueray*> bluerayHomeSearch(Client* homeTab, const QString& searchData);
    // 주문하기 클릭 시 발생 로직
    void bluerayOrdering(Client* bluerayTab);

private:
    BluerayManager* blueraymanager;
};

#endif // CLIENTBLUERAYSERVICE_H
