#include "order.h"

Order::Order() {}
Order::Order(const QString& userID, const QString& uuid, const int& price){
    this->setUserID(userID);
    this->setProd_uuid(uuid);
    this->setPrice(price);
}
Order::~Order(){}
Order* Order::getOrder(){
    return this;
}
void Order::setUserID(const QString& userID){
    this->userID = userID;
}
void Order::setProd_uuid(const QString& uuid){
    this->prod_uuid = uuid;
}
void Order::setPrice(const int& price){
    this->price = price;
}
