#ifndef PRODUCTINFO_H
#define PRODUCTINFO_H

#include "info.h"
#include <QImage>
#include <QBuffer>
#include <QByteArray>

class ProductInfo : public Info {
public:
    enum ProductType{
        Book,
        Blueray,
        Music,
        None
    };
    enum FilterType{
        Name,
        Author,
        Company,
        UUID
    };

    typedef struct filter{
        FilterType type;
        QString keyword;
        int minPrice;
        int maxPrice;
    } Filter;

public:
    ProductInfo();
    ~ProductInfo();

    QString getUuid();
    void setUuid(const QString& uuid);
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
    // image -- base64 디코딩 --> QString -> setter
    void setQImageToQString(const QImage& image);
    // QImage -- base64 디코딩 --> QString return
    QString getQImageToQStringReturn(const QImage& image);


private:
    QString Uuid;
    QString company;
    int price;
    QString context;
    int amount;
    // image -- base64 --> QString
    QString image;
};


#endif // PRODUCTINFO_H
