#ifndef COMMUINFO_H
#define COMMUINFO_H

#include "info.h"
#include "tcpcommudefines.h"
#include "userInfo.h"
#include "productInfo.h"

#include <QJsonDocument>
#include <QJsonArray>

class QFile;

class CommuInfo : public Info{
public:
    CommuInfo() = default;
    explicit CommuInfo(const QByteArray& commuInfo);
public:
    const QByteArray& GetByteArray() const { return byteArray; }

    CommuType GetType() const;

    Info::InfoType GetInfoType() const;
    ProductInfo::ProductType GetProductType() const;

    void SetChat(const QString& clientName, const QString& chat, QFile* file = nullptr, ChattingType chatType = ChattingType::General_ForAdmin, const QString& counterName = QString());
    void SetChat(const QString& clientName, const QString& chat, const QByteArray& fileBytes, const QString& fileName = QString(), ChattingType chatType = ChattingType::General_ForAdmin, const QString& counterName = QString());
    std::pair<QString, QString> GetChat(ChattingType& chatType, QString& counterName, QByteArray& fileBytes, QString& fileName) const;

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

    void ServerComfirmLoginOrOut(bool isLogin, const std::vector<QString>& names);
    std::vector<QString> GetConfirmLoginOrOut(bool& isLogin) const;
    // 서버에서 정제한 response 결과 배열을 bytearray 내부에 추가한다
    void AppendResponseArray(const QJsonArray& responseArray);
    void AppendResponseObject(const QJsonObject& responseObject);
    void RequestOrderProducts(ProductInfo::ProductType productType, const ProductInfo::Filter &filter, const QString& orderMenu);

    // file을 보낼때. sendType은 Admin이면 관리자에게만, 다른 경우는 유저한테.
    void FileFormat(const QString& SendType, QFile& file);
    // QByteArray 리턴값을 QFile로 치환한다.
    QByteArray GetFileBytes(QString& objective);
public:
    void AddSizePacket();
private:
    QByteArray byteArray;
};


#endif // COMMUINFO_H
