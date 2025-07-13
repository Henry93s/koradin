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
    // book 탭에서 book 검색 을 서버에 요청
    QVector<Book*> bookSearch(Client* bookTab);

    // 주문하기 클릭 시 발생 로직
    void bookOrdering(Client* bookTab);

private:
    BookManager* bookmanager;
};

#endif // CLIENTBOOKSERVICE_H
