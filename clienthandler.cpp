#include "clienthandler.h"

#include <QTcpSocket>
#include <server.h>
#include <QThread>
#include <QMessageBox>

ClientHandler::ClientHandler(QTcpSocket* socket,QObject *parent)
    : QObject{parent}, socket(socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);

}

void ClientHandler::onReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_0);

    while(1){
        if(expectedSize == 0){
            if(socket->bytesAvailable() < 4){
                qDebug() << "Break. clientHandler - bytesAvailable :" << socket->bytesAvailable() << " Expected Size :" << expectedSize;
                break; // 길이 정보가 아직 다 들어오지 않음
            }
            in >> expectedSize;
        }

        if(socket->bytesAvailable() < expectedSize){
            qDebug() << "Break2. clientHandler - bytesAvailable :" << socket->bytesAvailable() << " Expected Size :" << expectedSize;
            break; // 아직 데이터가 다 들어오지 않음
        }
        bytearray = socket->read(expectedSize);
        // 처리 코드
        emit response(thread(), bytearray);

        expectedSize = 0;
        bytearray.clear();
    }
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
