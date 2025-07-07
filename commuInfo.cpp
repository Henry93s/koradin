#include "commuInfo.h"
#include <qjsonparseerror.h>
#include <QJsonObject>
#include <QJsonArray>

CommuInfo::CommuInfo(const QByteArray &commuInfo) : byteArray{commuInfo}
{

}

CommuType CommuInfo::GetType() const
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &error);
    if (error.error == QJsonParseError::NoError && doc.isObject()) {
        QJsonObject obj = doc.object();
        auto commu = obj["CommuType"].toString();

        if(commu == QString("Infos")){
            return CommuType::Infos;
        }else if(commu == QString("Chatting")){
            return CommuType::Chatting;
        }else if(commu == QString("InfoFix")){
            return CommuType::InfoFix;
        }else if(commu == QString("AUTH")){
            return CommuType::AUTH;
        }
        else{
            return CommuType::COMMUEND;
        }
    }else{
        return CommuType::COMMUEND;
    }
}

std::pair<QString, QString> CommuInfo::GetChat() const
{
    if(GetType() != CommuType::Chatting){
        return {"",""};
    }

    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    QString usrname = data["UsrName"].toString();
    QString message = data["Message"].toString();
    return {usrname, message};
}

std::vector<UserInfo> CommuInfo::GetUsers() const
{
    if(GetType() != CommuType::Infos){
        return std::vector<UserInfo>();
    }

    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    QString infotype = data["InfoType"].toString();
    if(infotype != QString("User")){
        return std::vector<UserInfo>();
    }
    std::vector<UserInfo> ret;
    auto infos = data["Data"].toArray();
    for(const auto& value : infos){
        auto info = value.toObject();

        QString ID = info["ID"].toString();
        QString name = info["name"].toString();
        QString password = info["password"].toString();
        QString email = info["email"].toString();
        QString isAdmin = info["isAdmin"].toString();
        UserInfo user(ID, name, password, email, isAdmin);
        ret.push_back(user);
    }

    return ret;
}

std::pair<QString, QString> CommuInfo::GetIDPwd() const
{
    if(GetType() != CommuType::AUTH){
        qDebug() << GetType();
        return std::pair<QString, QString>();
    }

    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    return { data["ID"].toString(), data["password"].toString() };
}

void CommuInfo::AddUsers(std::vector<UserInfo>)
{
    QJsonObject whole;
    //whole["CommuType"] = tr("")
}
