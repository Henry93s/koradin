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

    void Initialize(const ClientData* client);

signals:

private:
    QLabel* usernameLabel;
};

#endif // USERITEM_H
