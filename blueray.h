#ifndef BLUERAY_H
#define BLUERAY_H

#include "productInfo.h"

class Blueray : public ProductInfo {
public:
    Blueray();
    Blueray(const QString& name, const QString& artist, const QString& company
          , const int& price, const QString& context, const int& amount, const QString& image);
    ~Blueray();
    QString getArtist();
    void setArtist(const QString& artist);

private:
    QString artist;
};

#endif // BLUERAY_H
