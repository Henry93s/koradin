#ifndef BLUERAY_H
#define BLUERAY_H

#include "productinfo.h"
#include "artist.h"
#include <QVector>

class Blueray : public ProductInfo {
public:
    Blueray();
    ~Blueray();
    QVector<Artist> getArtistList();
    void addArtist(QString& artist);
    QStringView minArtist(QString& artist);
    QStringView getPlot();
    void setPlot(QString& plot);

private:
    QVector<Artist> artistList;
    QString plot;
};

#endif // BLUERAY_H
