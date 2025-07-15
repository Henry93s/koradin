#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

#include "tcpcommudefines.h"

class CommuInfo;
#include "bookManager.h"
#include "bluerayManager.h"
#include "musicManager.h"
#include "clientBookService.h"
#include "clientMusicService.h"
#include "clientBluerayService.h"
#include "clientHomeService.h"

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

    // ClientService, clientBookService 등 각 탭의 Service 클래스에서 Ui 에 입력된 값을 요구할 수 있기 때문에 필요
    Ui::Client* getUi();
    clientdata* getClientData();

    void writeSocket(const QByteArray& bytes) { if(socket) socket->write(bytes); }

private slots:
    void chatForServer();

    // book tab button event
    void on_book_search_pushButton_clicked();
    void on_book_order_pushButton_clicked();

signals:
    void ChattingRespond(const CommuInfo& commuInfo);
    void InfosFixRespond(const CommuInfo& commuInfo);
private slots:
    void respond();
    // music tab button event
    void on_music_search_pushButton_clicked();
    void on_music_order_pushButton_clicked();

    // 블루레이 tab button event
    void on_blueray_search_pushButton_clicked();
    void on_blueray_order_pushButton_clicked();

    // 홈 tab button event
    void on_home_search_pushButton_clicked();
    void on_home_orderSearch_pushButton_clicked();

private:
    ClientData clientData;
    QTcpSocket* socket;

    Ui::Client *ui;
    BookManager* bookmanager;
    BluerayManager* blueraymanager;
    MusicManager* musicmanager;
    ClientBookService clientBookService;
    ClientBluerayService clientBluerayService;
    ClientMusicService clientMusicService;
    ClientHomeService clientHomeService;

    void InfosFetchRespond(const CommuInfo& commuInfo);
    void printBookSearchData(const CommuInfo& commuInfo);
    void printMusicSearchData(const CommuInfo& commuInfo);
    void printBlueraySearchData(const CommuInfo& commuInfo);

    // 다음에 수신해야 할 데이터의 바이트 수 (서버가 보내는 4바이트 길이 프리픽스를 기반으로 설정됨)
    quint32 expectedSize = 0;
    // 실제 수신된 JSON 또는 바이너리 데이터를 저장하는 버퍼
    QByteArray buffer;
    // 처음 client 실행 시에는 모든 product 들을 검색하고 setcurrentindex 가 동작을 안 하게 하기 위한 flag
    int is_first_search = 0;
};

#endif // CLIENT_H
