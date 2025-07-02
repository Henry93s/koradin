#ifndef BOOK_H
#define BOOK_H

#include "productInfo.h"

class Book : public ProductInfo {
public:
    Book();
    ~Book();
    QString getWriter();
    void setWriter(const QString& writer);

private:
    QString writer;
};

#endif // BOOK_H
