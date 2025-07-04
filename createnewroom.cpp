#include "createnewroom.h"
#include "ui_createnewroom.h"
#include "server.h"
#include "tcpcommudefines.h"

#include <QPushButton>
#include <QMessageBox>


CreateNewRoom::CreateNewRoom(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateNewRoom)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &CreateNewRoom::newRoomToParent);
}

CreateNewRoom::~CreateNewRoom()
{
    delete ui;
}

void CreateNewRoom::newRoomToParent()
{
    auto server = papa;

    if(server->GetRoomNum() == MAX_ROOMS){
        QMessageBox::critical(this, tr("Server"), tr("Rooms are Full!"));
        return;
    }

    RoomData room{ui->titleText->text(), -1};

    server->CreateNew_Room(room);

    this->close();
}
