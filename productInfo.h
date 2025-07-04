#ifndef PRODUCTINFO_H
#define PRODUCTINFO_H

#include "info.h"

class ProductInfo : public Info {
public:
    enum ProductType{
        BOOK,
        BLUERAY,
        MUSIC
    };

public:
    ProductInfo();
    ~ProductInfo();

    QString getTitle();
    void setTitle(const QString& title);
    QString getCompany();
    void setCompany(const QString& company);
    int getPrice();
    void setPrice(int price);
    QString getContext();
    void setContext(const QString& context);
    int getAmount();
    void setAmount(int amount);

private:
    QString title;
    QString company;
    int price;
    QString context;
    int amount;
};


#endif // PRODUCTINFO_H
