#ifndef PRODUCTINFO_H
#define PRODUCTINFO_H

#include "info.h"
#include <QString>

class ProductInfo : public Info {

public:
    ProductInfo();
    ~ProductInfo();

    QStringView getName();
    void setName(QString& name);
    int getPrice();
    void setPrice(int price);
    QStringView getContext();
    void setContext(QString& context);
    int getAmount();
    void setAmount(int amount);

private:
    QString name;
    int price;
    QString context;
    int amount;
};


#endif // PRODUCTINFO_H
