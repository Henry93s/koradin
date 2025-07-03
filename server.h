#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <productInfo.h>
#include <userInfo.h>
#include <QTcpServer>

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

private:
    Ui::Server *ui;
    QTabWidget* tabWidget;

    std::vector<std::shared_ptr<ProductInfo>> products;
    std::vector<std::shared_ptr<UserInfo>> users;
    //std::vector<std::shared_ptr<OrderInfo>> orders;

    void clientConnect();
    void echoData();

    QTcpServer* tcpServer;
private slots:
    void transferLabels(bool checked);
};
#endif // SERVER_H
