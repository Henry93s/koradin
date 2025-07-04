#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include <QMainWindow>
#include <productInfo.h>
#include <userInfo.h>
#include <commuInfo.h>
#include <queue>

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
private slots:
    void clientConnect();
    void clientDisconnected();
    void respond();

    void transferLabels(bool checked);

signals:
    void InfosFetchRespond(const CommuInfo& commuInfo);
    void ChattingRespond(const CommuInfo& commuInfo);
    void InfosFixRespond(const CommuInfo& commuInfo);

private:
    Ui::Server *ui;
    QTabWidget* tabWidget;

    QTcpServer* tcpServer;
    QVector<ClientData> clients;
    QVector<RoomData> rooms;

    std::vector<std::shared_ptr<ProductInfo>> products;
    std::vector<std::shared_ptr<UserInfo>> users;
};
#endif // SERVER_H
