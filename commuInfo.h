#ifndef COMMUINFO_H
#define COMMUINFO_H

#include "info.h"
#include "tcpcommudefines.h"
#include "userInfo.h"
#include "productInfo.h"

#include <QJsonDocument>
#include <QJsonArray>

class CommuInfo : public Info{
public:
    CommuInfo() = default;
    explicit CommuInfo(const QByteArray& commuInfo);
public:
    const QByteArray& GetByteArray() const { return byteArray; }

    CommuType GetType() const;

    Info::InfoType GetInfoType() const;
    ProductInfo::ProductType GetProductType() const;

    std::pair<QString, QString> GetChat() const;
    std::vector<std::pair<QString, QString>> GetUserIDAndNames() const;

    std::pair<QString, QString> GetIDPwd() const;

    // 클라가 서버한테 요청한다. 정보 보내줄 것을.
    void RequestProducts(ProductInfo::ProductType productType, const ProductInfo::Filter& filter);
    // 클라가 요청한 걸 서버가 읽는다.
    ProductInfo::ProductType GetRequestProducts(ProductInfo::Filter& filterRet) const;

    //이건 클라에서 가입한 회원을 서버로 보낼 떄 쓰는것.
    void AddUsers(std::vector<UserInfo>);
    std::vector<UserInfo> GetAddingUsers() const;
private:
    QByteArray byteArray;
};


#endif // COMMUINFO_H
