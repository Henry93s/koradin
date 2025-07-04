#ifndef CLIENTBOOKSERVICE_H
#define CLIENTBOOKSERVICE_H
#include "clientService.h"
#include "bookManager.h"

class ClientBookService : ClientService {
public:
    ClientBookService();
    ~ClientBookService();
    // 다중 검색 로직
    QVector<Book*> bookSearch(QWidget* bookTab);

private:
    BookManager* bookmanager;
};

#endif // CLIENTBOOKSERVICE_H
