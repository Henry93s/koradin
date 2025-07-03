#include "blueray.h"

#include <QString>

Blueray::Blueray(){}
Blueray::Blueray(const QString& name, const QString& artist, const QString& company
             , const int& price, const QString& context, const int& amount){
    this->setName(name);
    this->setArtist(artist);
    this->setCompany(company);
    this->setPrice(price);
    this->setContext(context);
    this->setAmount(amount);
}

QString Blueray::getArtist(){
    return this->artist;
}
void Blueray::setArtist(const QString& artist){
    this->artist = artist;
}
