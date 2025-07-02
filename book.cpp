#include "book.h"

Book::Book(){}
Book::~Book(){}

QString Book::getWriter(){
    return this->writer;
}
void Book::setWriter(const QString& writer){
    this->writer = writer;
}
