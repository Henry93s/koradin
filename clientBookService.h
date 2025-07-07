#ifndef CLIENTBOOKSERVICE_H
#define CLIENTBOOKSERVICE_H

#include "clientService.h"
#include "bookManager.h"

// class 끼리 중복 include 발생 -> clientBookService 에서는 Client 를 전방선언 처리
class Client;

class ClientBookService : ClientService {
public:
    ClientBookService();
    ~ClientBookService();
    // 다중 검색 로직
    QVector<Book*> bookSearch(Client* bookTab);
    // home 에서 통합 검색 요청으로 인한 search
    QVector<Book*> bookHomeSearch(const QString& searchData);
    // 주문하기 클릭 시 발생 로직
    void bookOrdering(Client* bookTab);

private:
    BookManager* bookmanager;
};

#endif // CLIENTBOOKSERVICE_H
