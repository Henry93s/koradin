#include "book.h"

Book::Book(){}
Book::~Book(){}

QStringView Book::getWriter(){
    return this->writer;
}
void Book::setWriter(QString& writer){
    this->writer = writer;
}
QStringView Book::getIndex(){
    return this->index;
}
void Book::setIndex(QString& index){
    this->index = index;
}
