#include "commuInfo.h"
#include <qjsonparseerror.h>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>
#include <QFile>

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
        }
        else if(commu == QString("Chatting")){
            return CommuType::Chatting;
        }
        else if(commu == QString("InfoFix")){
            return CommuType::InfoFix;
        }
        else if(commu == QString("InfoAdd")){
            return CommuType::InfoAdd;
        }
        else if(commu == QString("OrderInfos")){
            return CommuType::OrderInfos;
        }
        else if(commu == QString("OrderAdd")){
            return CommuType::OrderAdd;
        }
        else if(commu == QString("OrderDelete")){
            return CommuType::OrderDelete;
        }
        else if(commu == QString("AUTH")){
            return CommuType::AUTH;
        }
        else if(commu == QString("LOGINOUT")){
            return CommuType::LOGINOUT;
        }
        else if(commu == QString("File")){
            return CommuType::File;
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
    } else if(infotype == QString("Order")){
        return InfoType::Order;
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

void CommuInfo::SetChat(const QString &clientName, const QString &chat, QFile* file, ChattingType chatType, const QString& counterName)
{
    QJsonObject whole;
    QJsonObject Data;
    Data["UsrName"] = clientName;
    Data["Message"] = chat;

    switch(chatType){
    case ChattingType::General_ForAdmin:
        Data["ChatType"] = QString("General");
        break;
    case ChattingType::Whisper:
        Data["ChatType"] = QString("Whisper");
        Data["Counter"] = counterName;
        break;
    }

    whole["CommuType"] = QString("Chatting");
    whole["Data"] = Data;

    // 파일 넣기.
    if(file){
        QByteArray fileBytes;
        fileBytes = file->read(file->size());
        whole["File"] = QString::fromLatin1(fileBytes.toBase64());
        whole["FileName"] = file->fileName();
    }

    QJsonDocument doc{whole};
    byteArray = doc.toJson(QJsonDocument::Compact);
}

void CommuInfo::SetChat(const QString &clientName, const QString &chat, const QByteArray &fileBytes, const QString& fileName, ChattingType chatType, const QString &counterName)
{
    QJsonObject whole;
    QJsonObject Data;
    Data["UsrName"] = clientName;
    Data["Message"] = chat;

    switch(chatType){
    case ChattingType::General_ForAdmin:
        Data["ChatType"] = QString("General");
        break;
    case ChattingType::Whisper:
        Data["ChatType"] = QString("Whisper");
        Data["Counter"] = counterName;
        break;
    }

    whole["CommuType"] = QString("Chatting");
    whole["Data"] = Data;

    // 파일 넣기.
    if(!fileBytes.isEmpty()){
        whole["File"] = QString::fromLatin1(fileBytes.toBase64());
        whole["FileName"] = fileName;
    }

    QJsonDocument doc{whole};
    byteArray = doc.toJson(QJsonDocument::Compact);
}

std::pair<QString, QString> CommuInfo::GetChat(ChattingType& chatType, QString& counterName, QByteArray& fileBytes, QString& fileName) const
{
    if(GetType() != CommuType::Chatting){
        return {"",""};
    }

    QJsonParseError error;
    auto whole = QJsonDocument::fromJson(byteArray, &error).object();
    auto data = whole["Data"].toObject();
    QString usrname = data["UsrName"].toString();
    QString message = data["Message"].toString();
    counterName = data["Counter"].toString();
    QString chatTypeStr = data["ChatType"].toString();
    if(chatTypeStr == QString("General")){
        chatType = ChattingType::General_ForAdmin;
    }
    else if(chatTypeStr == QString("Whisper")){
        chatType = ChattingType::Whisper;
    }

    fileBytes = QByteArray::fromBase64(whole["File"].toString().toLatin1()); // retDoc.toJson(QJsonDocument::Compact);
    fileName = whole["FileName"].toString();

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

void CommuInfo::SetIDPwd(const QString &ID, const QString &password)
{
    QJsonObject obj;
    QJsonObject data;
    obj["CommuType"] = QString("AUTH");
    data["ID"] = ID;
    data["password"] = password;
    obj["Data"] = data;

    QJsonDocument doc(obj);
    byteArray = doc.toJson(QJsonDocument::Compact);
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
    case ProductInfo::FilterType::UUID:
        Filter["Type"] = QString("Uuid");
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

void CommuInfo::RequestOrderProducts(ProductInfo::ProductType productType, const ProductInfo::Filter &filter, const QString& orderMenu)
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
    case ProductInfo::FilterType::UUID:
        Filter["Type"] = QString("Uuid");
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
    Commu["CommuType"] = orderMenu;
    Commu["Info"] = Info;

    QJsonDocument docu(Commu);
    byteArray = docu.toJson(QJsonDocument::Compact);
}

ProductInfo::ProductType CommuInfo::GetRequestProducts(ProductInfo::Filter &filterRet) const
{
    if(!(GetType() == CommuType::Infos || GetType() == CommuType::OrderInfos || GetType() == CommuType::OrderAdd || GetType() == CommuType::OrderDelete)){
        qDebug() << "getRequestProducts() commuType : " << GetType();
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
    } else if(Filter["Type"] == QString("UUID")){
        filterRet.type = ProductInfo::FilterType::UUID;
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
        perObj["salt"] = user.getSalt();
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
        auto salt = perObj["salt"].toString();
        UserInfo info{id, name, pwd, email, isAdmin, salt};
        ret.push_back(info);
    }

    return ret;
}

void CommuInfo::LoginOrOut(bool isLogin, QString name, QString ID)
{
    QJsonObject obj;
    obj["CommuType"] = QString("LOGINOUT");
    if(isLogin){
        obj["LOG"] = QString("In");
    }
    else{
        obj["LOG"] = QString("Out");
    }
    obj["Name"] = name;
    obj["ID"] = ID;
    QJsonDocument doc(obj);
    byteArray = doc.toJson(QJsonDocument::Compact);
}

bool CommuInfo::GetLoginOrOut(QString &name, QString& ID) const
{
    QJsonParseError error;
    auto obj = QJsonDocument::fromJson(byteArray, &error).object();
    QString log = obj["LOG"].toString();
    bool ret;
    if(log == QString("In")){
        ret = true;
    }
    else {
        ret = false;
    }
    name = obj["Name"].toString();
    ID = obj["ID"].toString();
    return ret;
}

void CommuInfo::ServerComfirmLoginOrOut(bool isLogin, const std::vector<QString>& names)
{
    QJsonObject obj;
    obj["CommuType"] = QString("LOGINOUT");
    QJsonArray userArray;
    for(auto& n : names){
        QJsonObject perObj;
        perObj["name"] = n;
        userArray.append(perObj);
    }
    obj["Users"] = userArray;
    if(isLogin){
        obj["Log"] = QString("in");
    }else{
        obj["Log"] = QString("out");
    }

    QJsonDocument doc(obj);
    byteArray = doc.toJson(QJsonDocument::Compact);
}

std::vector<QString> CommuInfo::GetConfirmLoginOrOut(bool& isLogin) const
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if (err.error != QJsonParseError::NoError || GetType() != CommuType::LOGINOUT) {
        qDebug() << "JSON 파싱 실패:" << err.errorString();
        return {"", ""};
    }
    std::vector<QString> ret;
    QJsonObject obj = doc.object();
    const QJsonArray& userArray = obj["Users"].toArray();
    for(const auto& user : userArray)
    {
        auto perUser = user.toObject();
        ret.push_back(perUser["name"].toString());
    }
    auto log = obj["Log"].toString();

    // 로그인했는지 안했는지 구분
    if(log == QString("in")){
        isLogin = true;
    }
    else if(log == QString("out")){
        isLogin = false;
    }
    return ret;
}

void CommuInfo::AppendResponseArray(const QJsonArray& responseArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "JSON 파싱 실패:" << err.errorString();
        return;
    }

    QJsonObject obj = doc.object();
    obj["response"] = responseArray;

    QJsonDocument newDoc(obj);
    byteArray = newDoc.toJson(QJsonDocument::Compact);
}

void CommuInfo::FileFormat(const QString& sendType, QFile &file)
{
    QJsonObject obj;
    QByteArray fileBytes;
    obj["CommuType"] = QString("File");
    fileBytes = file.read(file.size());
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(fileBytes, &err);
    obj["File"] = doc.object();
    obj["Object"] = sendType;

    QJsonDocument newDoc(obj);
    byteArray = newDoc.toJson(QJsonDocument::Compact);
}

QByteArray CommuInfo::GetFileBytes(QString &objective)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if (err.error != QJsonParseError::NoError || GetType() != CommuType::File) {
        qDebug() << "JSON 파싱 실패:" << err.errorString();
        return {};
    }
    QJsonObject obj = doc.object();
    QJsonDocument retDoc = QJsonDocument(obj["File"].toObject());
    QByteArray ret = retDoc.toJson(QJsonDocument::Compact);
    objective = obj["Object"].toString();

    return ret;
}

void CommuInfo::AddSizePacket()
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << (quint32)byteArray.size(); // 길이 프리픽스
    packet.append(byteArray);
    byteArray = packet;
    // QByteArray packet;
    // QDataStream out(&packet, QIODevice::WriteOnly);
    // out.setVersion(QDataStream::Qt_6_0);
    // //out.setByteOrder(QDataStream::LittleEndian);
    // out << (quint32)byteArray.size();   // 먼저 길이 프리픽스
    // out.writeRawData(byteArray.constData(), byteArray.size()); // 그 다음 원 데이터

    // byteArray = packet;
}

void CommuInfo::AppendResponseObject(const QJsonObject& responseObject)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "JSON 파싱 실패:" << err.errorString();
        return;
    }

    QJsonObject obj = doc.object();
    obj["response"] = responseObject;

    QJsonDocument newDoc(obj);
    byteArray = newDoc.toJson(QJsonDocument::Compact);
}
