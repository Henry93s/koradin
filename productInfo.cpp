#include "productinfo.h"

ProductInfo::ProductInfo(){}
ProductInfo::~ProductInfo(){}

int ProductInfo::getPrice(){
    return this->price;
}
void ProductInfo::setPrice(int price){
    this->price = price;
}
QStringView ProductInfo::getContext(){
    return this->context;
}
void ProductInfo::setContext(QString& context){
    this->context = context;
}
