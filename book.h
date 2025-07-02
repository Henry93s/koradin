#ifndef BOOK_H
#define BOOK_H

#include "productInfo.h"

class Book : public ProductInfo {
public:
    Book();
    ~Book();
    QStringView getWriter();
    void setWriter(QString& writer);
    QStringView getIndex();
    void setIndex(QString& index);

private:
    QString writer;
    QString index;
};

#endif // BOOK_H
