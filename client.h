#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

#include "tcpcommudefines.h"

class CommuInfo;

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    void Initialize(QTcpSocket* sock, QStringView name = tr("None"));

private slots:
    void respond();
signals:
    void InfosFetchRespond(const CommuInfo& commuInfo);
    void ChattingRespond(const CommuInfo& commuInfo);
    void InfosFixRespond(const CommuInfo& commuInfo);
private:
    ClientData clientData;

    Ui::Client *ui;
};

#endif // CLIENT_H
