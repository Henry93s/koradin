#ifndef CREATENEWROOM_H
#define CREATENEWROOM_H

#include <QDialog>

class Server;

namespace Ui {
class CreateNewRoom;
}

class CreateNewRoom : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewRoom(QWidget *parent = nullptr);
    ~CreateNewRoom();

    void SetServer(Server* ser) { papa = ser; }
private:
    Ui::CreateNewRoom *ui;

    void newRoomToParent();

    Server * papa = nullptr;
};

#endif // CREATENEWROOM_H
