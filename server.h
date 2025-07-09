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
    void CreateNew_Room(const RoomData& newData);
    int GetRoomNum() const { return rooms.size(); }

private:
    void AUTHRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);
    void AddRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);
    void InfosFetchRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);

private slots:
    void clientConnect();
    void clientDisconnected();
    void respond();

    void transferLabels(bool checked);

    void enterRoom(RoomData& roomData);

signals:
    void ChattingRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);
    void InfosFixRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);
    void InfosAddRespond(const CommuInfo& commuInfo, QTcpSocket* clientConnection);
    //void AUTHRespond(const CommuInfo& commuInfo);
private:
    Ui::Server *ui;
    QTabWidget* tabWidget;

    QTcpServer* tcpServer;
    QVector<ClientData> clients;
    QVector<RoomData> rooms;

    //std::vector<std::shared_ptr<ProductInfo>> products;
    //std::vector<std::shared_ptr<UserInfo>> users;



    // Managers;
    UserManager userManager;
    MusicManager musicmanager;
    BluerayManager bluerayManager;
    BookManager bookManager;

    //void echoData();
};
#endif // SERVER_H
