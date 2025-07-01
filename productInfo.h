#ifndef PRODUCTINFO_H
#define PRODUCTINFO_H

#include "info.h"

class ProductInfo : public Info {

public:
    ProductInfo();
    ~ProductInfo();

    int getPrice();
    void setPrice(int price);
    QStringView getContext();
    void setContext(QString& context);
    int getAmount();
    void setAmount(int amount);

private:

    int price;
    QString context;
    int amount;
};


#endif // PRODUCTINFO_H
