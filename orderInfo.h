#ifndef ORDERINFO_H
#define ORDERINFO_H
#include "info.h"
#include "userInfo.h"
#include "productInfo.h"
#include <QVector>
#include <QMap>

class OrderInfo : public Info{
public:
    OrderInfo();
    ~OrderInfo();
    UserInfo* getOrderer();
    int getPriceSum();
    void setOrderer(UserInfo* user);
    void addPriceSum(int price);
    void minusPriceSum(int price);
    void setOrderItems(QVector<ProductInfo*> orderItems);
    QVector<ProductInfo*> getOrderItems();
    void addOrderItem(ProductInfo* product);
    void minusOrderItem(ProductInfo* product);

    QJsonObject toJsonObject() const;
    static OrderInfo* fromJsonObject(const QJsonObject& obj);

private:
    UserInfo* orderer;
    QVector<ProductInfo*> orderItems;
    int priceSum = 0;
};

#endif // ORDERINFO_H
