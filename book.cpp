#include "book.h"
#include <QJsonObject>

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

QJsonObject Book::toJsonObject() {
    QJsonObject obj;
    obj["uuid"] = this->getUuid();
    obj["name"] = this->getName();
    obj["writer"] = this->getWriter();
    obj["company"] = this->getCompany();
    obj["price"] = this->getPrice();
    obj["amount"] = this->getAmount();
    obj["context"] = this->getContext();
    obj["image"] = this->getQImagePureQString();
    return obj;
}

Book* Book::fromJsonObject(const QJsonObject& obj) {
    return new Book(
        obj["uuid"].toString(),
        obj["name"].toString(),
        obj["writer"].toString(),
        obj["company"].toString(),
        obj["price"].toInt(),
        obj["context"].toString(),
        obj["amount"].toInt(),
        obj["image"].toString()
        );
}
