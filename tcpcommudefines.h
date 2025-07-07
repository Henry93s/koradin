#ifndef TCPCOMMUDEFINES_H
#define TCPCOMMUDEFINES_H

#include <QTcpSocket>

#define PORT    5085
#define MAX_CLIENTS 20
#define MAX_ROOMS   20
#define BLOCK_SIZE 1024

enum CommuType{
    Chatting,
    Infos,
    InfoFix,
    InfoAdd,
    AUTH,
    COMMUEND
};

typedef struct clientdata{
    QTcpSocket* socket = nullptr;
    int room_idx = -1; // 같은 room_idx 끼리 채팅 대화 진행됨 기본 0
    QString name; // 접속한 클라이언트 이름
} ClientData;

typedef struct roomdata{
    QString name;
    int index;
    std::vector<const ClientData*> clients;
    std::vector<std::pair<QString, QString>> messages;
} RoomData;

#endif // TCPCOMMUDEFINES_H
