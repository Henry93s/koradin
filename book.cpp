#include "book.h"

Book::Book(){}
Book::Book(const QString& uuid, const QString& name, const QString& writer, const QString& company
             , const int& price, const QString& context, const int& amount, const QString& image){
    this->setUuid(uuid);
    this->setName(name);
    this->setWriter(writer);
    this->setCompany(company);
    this->setPrice(price);
    this->setContext(context);
    this->setAmount(amount);
    this->setQImagePureQString(image);
}
Book::~Book(){}

QString Book::getWriter(){
    return this->writer;
}
void Book::setWriter(const QString& writer){
    this->writer = writer;
}
