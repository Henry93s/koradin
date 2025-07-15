#include "useritem.h"

#include <QLabel>

UserItem::UserItem(QWidget *parent)
    : QWidget{parent}
{}

void UserItem::Initialize(ClientData *client)
{
    pointingClient = client;
    usernameLabel = new QLabel(client->name, this);
}
