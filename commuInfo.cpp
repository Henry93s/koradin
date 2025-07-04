#include "commuInfo.h"
#include <qjsonparseerror.h>
#include <QJsonObject>

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

        if(commu == "Infos"){
            return CommuType::Infos;
        }else if(commu == "Chatting"){
            return CommuType::Chatting;
        }else if(commu == "InfoFix"){
            return CommuType::InfoFix;
        }else{
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
