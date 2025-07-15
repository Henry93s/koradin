#ifndef ORDERINFO_H
#define ORDERINFO_H

#include "info.h"

class UserInfo;
class ProductInfo;
class QJsonObject;

class OrderInfo : public Info{
public:
    OrderInfo(const QJsonObject& jsonObj);
    ~OrderInfo();


private:
    UserInfo* orderer;
    std::vector<ProductInfo*> orderedProducts;
    int priceSum = 0;
};

#endif // ORDERINFO_H
