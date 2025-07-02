#include "productinfo.h"

ProductInfo::ProductInfo(){}
ProductInfo::~ProductInfo(){}

QString ProductInfo::getTitle(){
    return this->title;
}
void ProductInfo::setTitle(const QString& title){
    this->title = title;
}

int ProductInfo::getPrice(){
    return this->price;
}
void ProductInfo::setPrice(int price){
    this->price = price;
}
QString ProductInfo::getContext(){
    return this->context;
}
void ProductInfo::setContext(const QString& context){
    this->context = context;
}
