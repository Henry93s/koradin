#ifndef COMMUINFO_H
#define COMMUINFO_H

#include "info.h"
#include "tcpcommudefines.h"
#include "userInfo.h"

#include <QJsonDocument>

class CommuInfo : public Info{
public:
    explicit CommuInfo(const QByteArray& commuInfo);
public:

    CommuType GetType() const;

    std::pair<QString, QString> GetChat() const;
    std::vector<UserInfo> GetUsers() const;

    std::pair<QString, QString> GetIDPwd() const;

    void AddUsers(std::vector<UserInfo>);

private:
    QByteArray byteArray;
};


#endif // COMMUINFO_H
