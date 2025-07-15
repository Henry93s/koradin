#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>

class QTcpSocket;
class Server;

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QTcpSocket* socket, QObject *parent = nullptr);

private:
    void onReadyRead();

    void onDisconnected();
public slots:
    void send(const QThread* compareThread, const QByteArray& data);
signals:
    void response(const QThread* thread, const QByteArray& readByte);
    void disConnected(const QThread* thread);
private:
    QTcpSocket* socket;
};

#endif // CLIENTHANDLER_H
