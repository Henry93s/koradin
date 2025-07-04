#ifndef COMMUINFO_H
#define COMMUINFO_H

#include "info.h"
#include "tcpcommudefines.h"
#include <QJsonDocument>

class CommuInfo : public Info{
public:
    explicit CommuInfo(const QByteArray& commuInfo);
public:

    CommuType GetType() const;

    std::pair<QString, QString> GetChat() const;
private:
    QByteArray byteArray;
};


#endif // COMMUINFO_H
