#ifndef ARTIST_H
#define ARTIST_H

#include "productinfo.h"

class Artist : public ProductInfo {
public:
    Artist();
    Artist(QString& artist);
    ~Artist();
};

#endif // ARTIST_H
