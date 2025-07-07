#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

#include "tcpcommudefines.h"

class CommuInfo;
#include "bookManager.h"
#include "clientBookService.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    void Initialize(QTcpSocket* sock, const QString& name = tr("None"));
    void printBookList();
    // ClientService, clientBookService 등 각 탭의 Service 클래스에서 Ui 에 입력된 값을 요구할 수 있기 때문에 필요
    Ui::Client* getUi();
    void printSearchBookList(const QVector<Book*>& list);

private slots:
    void on_book_search_pushButton_clicked();

    void on_book_order_pushButton_clicked();

private slots:
    void respond();
signals:
    void InfosFetchRespond(const CommuInfo& commuInfo);
    void ChattingRespond(const CommuInfo& commuInfo);
    void InfosFixRespond(const CommuInfo& commuInfo);
private:
    ClientData clientData;

    Ui::Client *ui;
    BookManager* bookmanager;
    ClientBookService clientBookService;
};

#endif // CLIENT_H
