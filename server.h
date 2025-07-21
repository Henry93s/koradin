#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include <QMainWindow>
#include <productInfo.h>
#include <userInfo.h>
#include <commuInfo.h>
#include <userManager.h>
#include <musicManager.h>
#include <bookManager.h>
#include <bluerayManager.h>
#include <orderManager.h>
#include <logManager.h>

class QTabWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class Server;
}
QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();

public:
    void Initialize();
    void CreateNew_Room(const RoomData& newData);
    int GetRoomNum() const { return rooms.size(); }

private:
    void AUTHRespond(const CommuInfo& commuInfo, ClientData* client);
    void AddRespond(const CommuInfo& commuInfo, ClientData* client);
    void InfosFetchRespond(const CommuInfo& commuInfo, ClientData* client);
    void LoginOutRespond(const CommuInfo& commuInfo, ClientData* client);
    void ChattingRespond(const CommuInfo& commuInfo, ClientData* client);
    void OrderInfosFetchRespond(const CommuInfo &commuInfo, ClientData* client);
    void OrderAddRespond(const CommuInfo& commuInfo, ClientData* client);
    void OrderDeleteRespond(const CommuInfo& commuInfo, ClientData* client);

    void SearchDataResponse(const CommuInfo& commuInfo, ClientData* client);
    void SearchOrderDataResponse(const CommuInfo& commuInfo, ClientData* client);
    void AddOrderDataResponse(const CommuInfo& commuInfo, ClientData* client);
    void DeleteOrderDataResponse(const CommuInfo& commuInfo, ClientData* client);

    void UpdateUI(Info::InfoType type, ProductInfo::ProductType ifProductType = ProductInfo::None);
    void UpdateUI_Product(ProductInfo::ProductType productType);
    void UpdateUI_Product_Book();
    void UpdateUI_Product_Blueray();
    void UpdateUI_Product_Music();

public slots:
    void respond(const QThread* thread, QByteArray bytearray);
    void clientDisconnected(const QThread* thread);

private slots:
    void clientConnect();

    void transferLabels(bool checked);

    void enterRoom(int roomIndex);

signals:
    void writeReady(const QThread* compareThread, const QByteArray& byte);
    void InfosFixRespond(const CommuInfo& commuInfo, ClientData* client);
    void InfosAddRespond(const CommuInfo& commuInfo, ClientData* client);
    //void AUTHRespond(const CommuInfo& commuInfo);

    void signalForBookUI(const std::vector<Book*> books);
    void signalForBluerayUI(const std::vector<Blueray*> bluerays);
    void signalForMusicUI(const std::vector<Music*> musics);

private:
    int currentRoomIndex = -1;
    Ui::Server *ui;
    QTabWidget* tabWidget;

    QTcpServer* tcpServer;
    QVector<ClientData> clients;
    QVector<RoomData> rooms;

    std::vector<UserInfo*> users;
    std::vector<Music*> musics;
    std::vector<Blueray*> bluerays;
    std::vector<Book*> books;

    // Managers;
    UserManager* userManager;
    MusicManager* musicManager;
    BluerayManager* bluerayManager;
    BookManager* bookManager;
    OrderManager* orderManager;
    LogManager* logManager;

    //void echoData();
};
#endif // SERVER_H
