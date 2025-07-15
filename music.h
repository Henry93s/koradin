#ifndef MUSIC_H
#define MUSIC_H

#include "productInfo.h"

class Music : public ProductInfo{
public:
    Music();
    Music(const QString& uuid, const QString& name, const QString& artist, const QString& company
            , const int& price, const QString& context, const int& amount, const QString& image);
    ~Music();
    QString getArtist();
    void setArtist(const QString& artist);

    QJsonObject toJsonObject();
    static Music* fromJsonObject(const QJsonObject& obj);

private:
    QString artist;
};


#endif // MUSIC_H
