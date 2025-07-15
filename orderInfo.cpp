#include "orderInfo.h"
#include "userManager.h"
// #include "musicManager.h"
// #include "bookManager.h"
// #include "bluerayManager.h"

#include <QJsonObject>
#include <QJsonArray>

OrderInfo::OrderInfo(const QJsonObject& jsonObj) : Info()
{
    QString userId = jsonObj["User"].toString();
    orderer = UserManager::getInstance()->userSearchById(userId);
    const QJsonArray& productArray = jsonObj["Products"].toArray();
    for(const auto& product : productArray){
        QString productId = product.toString();

        //BookManager::
    }
}
OrderInfo::~OrderInfo(){}
