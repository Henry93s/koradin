#include "blueray.h"

#include <QString>

QString Blueray::getArtist(){
    return this->artist;
}
void Blueray::setArtist(const QString& artist){
    this->artist = artist;
}
