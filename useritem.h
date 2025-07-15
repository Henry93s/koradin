#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

#include "tcpcommudefines.h"

class QLabel;

class UserItem : public QWidget
{
    Q_OBJECT
public:
    explicit UserItem(QWidget *parent = nullptr);

    void Initialize(ClientData* client);

    const ClientData* GetPointingClient() const {return pointingClient;}
signals:

private:
    QLabel* usernameLabel;
    ClientData* pointingClient;
};

#endif // USERITEM_H
