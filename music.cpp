#include "music.h"

Music::Music(){}
Music::Music(const QString& name, const QString& artist, const QString& company
             , const int& price, const QString& context, const int& amount, const QString& image){
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
