#include "commuInfo.h"
#include <qjsonparseerror.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>

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
        }else if(commu == QString("InfoAdd")){
            return CommuType::InfoAdd;
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

Info::InfoType CommuInfo::GetInfoType() const
{
    if(GetType() != CommuType::Infos){
        return InfoType::None;
    }

    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    QString infotype = data["InfoType"].toString();
    if(infotype == QString("User")){
        return InfoType::User;
    }else if(infotype == QString("Product")){
        return InfoType::Product;
    } else if(infotype == QString("Music")){
        return InfoType::Music;
    } else {
        return InfoType::None;
    }
}

ProductInfo::ProductType CommuInfo::GetProductType() const
{
    if(GetInfoType() != InfoType::Product){
        return ProductInfo::ProductType::None;
    }
    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    //QString infotype = data["InfoType"].toString();
    auto dataOfData = data["Data"].toObject();
    QString ProductType = dataOfData["ProductType"].toString();
    if(ProductType == QString("Book")){
        return ProductInfo::Book;
    } else if(ProductType == QString("Blueray")){
        return ProductInfo::Blueray;
    } else if(ProductType == QString("Music")){
        return ProductInfo::Music;
    } else{
        return ProductInfo::None;
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

std::vector<std::pair<QString, QString>> CommuInfo::GetUserIDAndNames() const
{
    if(GetType() != CommuType::Infos){
        return std::vector<std::pair<QString, QString>>();
    }

    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    QString infotype = data["InfoType"].toString();
    if(infotype != QString("User")){
        return std::vector<std::pair<QString, QString>>();
    }
    std::vector<std::pair<QString, QString>> ret;
    const QJsonArray& infos = data["Data"].toArray();
    for(const QJsonValue& value : infos){
        auto info = value.toObject();

        QString ID = info["ID"].toString();
        QString name = info["name"].toString();
        ret.push_back({ID, name});
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

void CommuInfo::RequestProducts(ProductInfo::ProductType productType, const ProductInfo::Filter &filter)
{
    QJsonObject Filter;
    switch(filter.type){
    case ProductInfo::FilterType::Name:
        Filter["Type"] = QString("Name");
        break;
    case ProductInfo::FilterType::Author:
        Filter["Type"] = QString("Author");
        break;
    case ProductInfo::FilterType::Company:
        Filter["Type"] = QString("Company");
        break;
    }
    Filter["Keyword"] = filter.keyword;
    Filter["Min"] = filter.minPrice;
    Filter["Max"] = filter.maxPrice;

    QJsonObject Product;
    switch(productType){
    case ProductInfo::Book:
        Product["ProductType"] = QString("Book");
        break;
    case ProductInfo::Blueray:
        Product["ProductType"] = QString("Blueray");
        break;
    case ProductInfo::Music:
        Product["ProductType"] = QString("Music");
        break;
    default:
        return;
        break;
    }
    Product["Filter"] = Filter;
    QJsonObject Info;
    Info["InfoType"] = QString("Product");
    Info["Product"] = Product;
    QJsonObject Commu;
    Commu["CommuType"] = QString("Infos");
    Commu["Info"] = Info;

    QJsonDocument docu(Commu);
    byteArray = docu.toJson(QJsonDocument::Compact);
}

ProductInfo::ProductType CommuInfo::GetRequestProducts(ProductInfo::Filter &filterRet) const
{
    if(GetType() != CommuType::Infos){
        qDebug() << GetType();
        return ProductInfo::ProductType::None;
    }

    QJsonParseError error;
    auto info = QJsonDocument::fromJson(byteArray, &error).object()["Info"].toObject();
    if(info["InfoType"] != QString("Product")){
        return ProductInfo::ProductType::None;
    }
    QJsonObject Product = info["Product"].toObject();

    QJsonObject Filter = Product["Filter"].toObject();
    filterRet.keyword = Filter["Keyword"].toString();
    filterRet.minPrice = Filter["Min"].toInt();
    filterRet.maxPrice = Filter["Max"].toInt();
    if(Filter["Type"] == QString("Name")){
        filterRet.type = ProductInfo::FilterType::Name;
    } else if(Filter["Type"] == QString("Author")){
        filterRet.type = ProductInfo::FilterType::Author;
    } else if(Filter["Type"] == QString("Company")){
        filterRet.type = ProductInfo::FilterType::Company;
    }

    if(Product["ProductType"] == QString("Book")){
        return ProductInfo::ProductType::Book;
    } else if(Product["ProductType"] == QString("Blueray")){
        return ProductInfo::ProductType::Blueray;
    } else if(Product["ProductType"] == QString("Music")){
        return ProductInfo::ProductType::Music;
    } else{
        return ProductInfo::ProductType::None;
    }
}

void CommuInfo::AddUsers(std::vector<UserInfo> users)
{
    QJsonObject whole;
    QJsonObject data;

    QJsonArray userArray;
    for(auto& user : users){
        QJsonObject perObj;
        perObj["ID"] = user.getID();
        perObj["name"] = user.getName();
        perObj["password"] = user.getPassword();
        perObj["email"] = user.getEmail();
        perObj["isAdmin"] = user.getIsAdmin();
        userArray.push_back(perObj);
    }

    data["InfoType"] = QString("User");
    data["Data"] = userArray;
    whole["CommuType"] = QString("InfoAdd");
    whole["Data"] = data;

    QJsonDocument docu(whole);
    byteArray = docu.toJson(QJsonDocument::Compact);
}

std::vector<UserInfo> CommuInfo::GetAddingUsers() const
{
    qDebug("GetAddingUsers");
    if(GetType() != CommuType::InfoAdd){
        qDebug() << GetType();
        return std::vector<UserInfo>();
    }

    std::vector<UserInfo> ret;
    QJsonParseError error;
    auto data = QJsonDocument::fromJson(byteArray, &error).object()["Data"].toObject();
    const QJsonArray& array = data["Data"].toArray();

    for(const QJsonValue& ref : array){
        auto perObj = ref.toObject();
        auto id = perObj["ID"].toString();
        auto name = perObj["name"].toString();
        auto pwd = perObj["password"].toString();
        auto email = perObj["email"].toString();
        auto isAdmin = perObj["isAdmin"] .toString();
        UserInfo info{id, name, pwd, email, isAdmin};
        ret.push_back(info);
    }

    return ret;
}
