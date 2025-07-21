#include "clienthandler.h"

#include <QTcpSocket>
#include <server.h>
#include <QThread>

ClientHandler::ClientHandler(QTcpSocket* socket,QObject *parent)
    : QObject{parent}, socket(socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);

}

void ClientHandler::onReadyRead()
{
    QByteArray data = socket->readAll();
    // 처리 코드
    emit response(thread(), data);
}

void ClientHandler::onDisconnected()
{
    emit disConnected(thread());
    socket->deleteLater();
    this->deleteLater();  // handler 제거
}

void ClientHandler::send(const QThread* compareThread, const QByteArray &data)
{
    if(thread() != compareThread)
    {
        // qDebug() << "일치안함 :" <<thread() << " and " << compareThread;
        return;
    }
    // qDebug() << "일치함 :" << thread();

    socket->write(data);
}
