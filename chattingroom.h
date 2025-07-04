#ifndef CHATTINGROOM_H
#define CHATTINGROOM_H

#include <QWidget>
#include "tcpcommudefines.h"

class QTcpSocket;
class QListWidget;
class QStackedWidget;

namespace Ui {
class ChattingRoom;
}

class ChattingRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ChattingRoom(QWidget *parent = nullptr);
    ~ChattingRoom();

    void SetRoomData(RoomData* room);
    void AddClientData(const ClientData* data) {roomData->clients.push_back(data);}
    void DeleteClientData(const ClientData* data);

    void SetStacked(QStackedWidget* stacked) { this->stacked = stacked; }

    QListWidget* GetChattingList();
private:
    Ui::ChattingRoom *ui;

    RoomData* roomData;

    QStackedWidget* stacked;
    //std::vector<const ClientData*> clientDatas;
};

#endif // CHATTINGROOM_H
