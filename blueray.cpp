#include "blueray.h"
#include "artist.h"

#include <QString>

QVector<Artist> Blueray::getArtistList(){
    return this->artistList;
}
void Blueray::addArtist(QString& artist){
    Artist temp(artist);
    this->artistList.append(temp);
}
QStringView Blueray::minArtist(QString& artist){
    QStringView temp;
    QString not_found = "not found";
    int is_find = 0;
    for(auto i = artistList.begin(); i != artistList.end();){
        if(artist.compare(i->getName()) == 0){
            is_find = 1;
            temp = i->getName();
            artistList.erase(i);
        } else {
            i++;
        }
    }

    if(is_find) return temp;
    else return (QStringView)not_found;
}
QStringView Blueray::getPlot(){
    return this->plot;
}
void Blueray::setPlot(QString& plot){
    this->plot = plot;
}
