#ifndef PRODUCTINFO_H
#define PRODUCTINFO_H

#include "info.h"
#include <QImage>
#include <QBuffer>
#include <QByteArray>

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

    QString getCompany();
    void setCompany(const QString& company);
    int getPrice();
    void setPrice(int price);
    QString getContext();
    void setContext(const QString& context);
    int getAmount();
    void setAmount(int amount);
    // QString return
    QString getQImagePureQString();
    // QString input
    void setQImagePureQString(const QString& image);
    // QString -- base64 인코딩 --> QImage
    QImage getQImagefromQString(const QString& imageString);
    // image -- base64 디코딩 --> QString -> set
    void setQImageToQString(const QImage& image);

private:
    QString company;
    int price;
    QString context;
    int amount;
    // image -- base64 --> QString
    QString image;
};


#endif // PRODUCTINFO_H
