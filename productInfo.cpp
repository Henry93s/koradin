#include "productinfo.h"

ProductInfo::ProductInfo(){}
ProductInfo::~ProductInfo(){}

QStringView ProductInfo::getProductName(){
    return this->productName;
}
void ProductInfo::setProductName(QString& productName){
    this->productName = productName;
}
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
