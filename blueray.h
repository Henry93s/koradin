#ifndef BLUERAY_H
#define BLUERAY_H

#include "productinfo.h"

class Blueray : public ProductInfo {
public:
    Blueray();
    ~Blueray();
    QString getArtist();
    void setArtist(const QString& artist);

private:
    QString artist;
};

#endif // BLUERAY_H
