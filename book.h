#ifndef BOOK_H
#define BOOK_H

#include "productInfo.h"

class Book : public ProductInfo {
public:
    Book();
    Book(const QString& uuid, const QString& name, const QString& writer, const QString& company
          , const int& price, const QString& context, const int& amount, const QString& image);
    ~Book();
    QString getWriter();
    void setWriter(const QString& writer);

private:
    QString writer;
};

#endif // BOOK_H
