#ifndef MUSIC_H
#define MUSIC_H

#include "productinfo.h"

class Music : public ProductInfo{
public:
    Music();
    ~Music();
    QString getArtist();
    void setArtist(const QString& Artist);

private:
    QString artist;
};


#endif // MUSIC_H
