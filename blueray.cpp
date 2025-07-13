#include "blueray.h"

#include <QString>
#include <QJsonObject>

Blueray::Blueray(){}
Blueray::Blueray(const QString& uuid, const QString& name, const QString& artist, const QString& company
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

QString Blueray::getArtist(){
    return this->artist;
}
void Blueray::setArtist(const QString& artist){
    this->artist = artist;
}

QJsonObject Blueray::toJsonObject() {
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

Blueray* Blueray::fromJsonObject(const QJsonObject& obj) {
    return new Blueray(
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

