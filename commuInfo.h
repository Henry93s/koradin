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

    void SetChat(const QString& clientName, const QString& chat);
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

    // true 넣으면 로그인 알림, false 넣으면 로그아웃 알림
    void LoginOrOut(bool isLogin, QString name, QString ID);
    bool GetLoginOrOut(QString& name, QString& ID) const;

    void ServerComfirmLoginOrOut(bool isLogin);
    // 서버에서 정제한 response 결과 배열을 bytearray 내부에 추가한다
    void AppendResponseArray(const QJsonArray& responseArray);
    void AppendResponseObject(const QJsonObject& responseObject);
    void RequestOrderProducts(ProductInfo::ProductType productType, const ProductInfo::Filter &filter);

private:
    QByteArray byteArray;
};


#endif // COMMUINFO_H
