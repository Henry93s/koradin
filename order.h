#ifndef ORDER_H
#define ORDER_H
#include <QString>

class Order
{
public:
    Order();
    Order(const QString& userID, const QString& uuid, const int& price);
    ~Order();

    QString getUserID();
    QString getProd_uuid();
    int getPrice();
    Order* getOrder();

    void setUserID(const QString& userID);
    void setProd_uuid(const QString& uuid);
    void setPrice(const int& price);

private:
    QString userID;
    QString prod_uuid;
    int price;
};

#endif // ORDER_H
