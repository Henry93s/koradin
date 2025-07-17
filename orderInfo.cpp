#include "orderInfo.h"
#include <QJsonArray>
#include <QJsonObject>
#include "userManager.h"
#include "bookManager.h"
#include "bluerayManager.h"
#include "musicManager.h"

OrderInfo::OrderInfo() : Info()
{

    /*
    QString userID = jsonObj["userID"].toString();
    orderer = UserManager::getInstance()->userSearchById(userID);
    const QJsonArray& orderItemArray = jsonObj["orderItems"].toArray();
    for(const auto& item : orderItemArray){
        QString uuid = item.toString();

    }
    */
}
OrderInfo::~OrderInfo(){}

UserInfo* OrderInfo::getOrderer(){
    return this->orderer;
}

int OrderInfo::getPriceSum(){
    return this->priceSum;
}

void OrderInfo::setOrderItems(QVector<ProductInfo*> orderItems){
    this->orderItems = orderItems;
}

QVector<ProductInfo*> OrderInfo::getOrderItems(){
    return this->orderItems;
}

void OrderInfo::setOrderer(UserInfo* user){
    this->orderer = user;
}

void OrderInfo::addPriceSum(int price){
    this->priceSum += price;
}

void OrderInfo::minusPriceSum(int price){
    this->priceSum -= price;
}

void OrderInfo::addOrderItem(ProductInfo* product) {
    for (ProductInfo* p : this->orderItems) {
        if (p->getUuid() == product->getUuid()) {
            return;
        }
    }
    this->addPriceSum(product->getPrice());
    this->orderItems.append(product);
}

void OrderInfo::minusOrderItem(ProductInfo* product){
    for(auto i = this->orderItems.begin(); i != this->orderItems.end();){
        if((*i)->getUuid().compare(product->getUuid()) == 0){
            this->minusPriceSum((*i)->getPrice());
            i = this->orderItems.erase(i);
            break;
        }
    }
}

QJsonObject OrderInfo::toJsonObject() const {
    QJsonObject obj;

    if (this->orderer) {
        obj["userID"] = this->orderer->getID();
    }

    QJsonArray orderArray;
    for (ProductInfo* product : this->orderItems) {
        QJsonObject itemObj;
        itemObj["uuid"] = product->getUuid();
        orderArray.append(itemObj);
    }
    obj["orderItems"] = orderArray;

    obj["priceSum"] = this->priceSum;

    return obj;
}

OrderInfo* OrderInfo::fromJsonObject(const QJsonObject& obj) {
    OrderInfo* orderInfo = new OrderInfo();

    // user
    QString userId = obj["userID"].toString();
    if (!userId.isEmpty()) {
        UserInfo* user = UserManager::getInstance()->userSearchById(userId);
        if (user) {
            orderInfo->setOrderer(user);
        }
    }

    // orderitmes
    QJsonArray orderArray = obj["orderItems"].toArray();
    QVector<ProductInfo*> items;

    for (const QJsonValue& val : orderArray) {
        QString uuid = val.toObject()["uuid"].toString();
        if (uuid.isEmpty()) continue;

        ProductInfo* product = nullptr;

        // 각 manager 에서 UUID 찾음
        if (!product) product = BookManager::getInstance()->bookSearchByUuid(uuid);
        if (!product) product = BluerayManager::getInstance()->blueraySearchByUuid(uuid);
        if (!product) product = MusicManager::getInstance()->musicSearchByUuid(uuid);

        if (product) {
            items.append(product);
        }
    }
    orderInfo->setOrderItems(items);

    orderInfo->addPriceSum(obj["priceSum"].toInt());

    return orderInfo;
}
