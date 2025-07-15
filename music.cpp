#include "music.h"
#include <QJsonObject>

Music::Music(){}
Music::Music(const QString& uuid, const QString& name, const QString& artist, const QString& company
             , const int& price, const QString& context, const int& amount, const QString& image){
    this->setUuid(uuid);
    this->setName(name);
    this->setArtist(artist);
    this->setCompany(company);
    this->setPrice(price);
    this->setContext(context);
    this->setAmount(amount);
    this->setQImagePureQString(image);
}
Music::~Music(){}

QString Music::getArtist(){
    return this->artist;
}
void Music::setArtist(const QString& artist){
    this->artist = artist;
}

QJsonObject Music::toJsonObject() {
    QJsonObject obj;
    obj["uuid"] = this->getUuid();
    obj["name"] = this->getName();
    obj["artist"] = this->getArtist();
    obj["company"] = this->getCompany();
    obj["price"] = this->getPrice();
    obj["amount"] = this->getAmount();
    obj["context"] = this->getContext();
    obj["image"] = this->getQImagePureQString();
    return obj;
}

Music* Music::fromJsonObject(const QJsonObject& obj) {
    return new Music(
        obj["uuid"].toString(),
        obj["name"].toString(),
        obj["artist"].toString(),
        obj["company"].toString(),
        obj["price"].toInt(),
        obj["context"].toString(),
        obj["amount"].toInt(),
        obj["image"].toString()
        );
}
