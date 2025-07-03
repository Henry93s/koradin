#include "book.h"

Book::Book(){}
Book::Book(const QString& name, const QString& writer, const QString& company
             , const int& price, const QString& context, const int& amount){
    this->setName(name);
    this->setWriter(writer);
    this->setCompany(company);
    this->setPrice(price);
    this->setContext(context);
    this->setAmount(amount);
}
Book::~Book(){}

QString Book::getWriter(){
    return this->writer;
}
void Book::setWriter(const QString& writer){
    this->writer = writer;
}
