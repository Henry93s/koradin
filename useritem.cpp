#include "useritem.h"

#include <QLabel>

UserItem::UserItem(QWidget *parent)
    : QWidget{parent}
{}

void UserItem::Initialize(const ClientData *client)
{
    usernameLabel = new QLabel(client->name, this);
}
