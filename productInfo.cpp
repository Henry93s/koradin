#include "productinfo.h"

ProductInfo::ProductInfo(){}
ProductInfo::~ProductInfo(){}

QString ProductInfo::getCompany(){
    return this->company;
}
void ProductInfo::setCompany(const QString& company){
    this->company = company;
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

int ProductInfo::getAmount(){
    return this->amount;
}
void ProductInfo::setAmount(int amount){
    this->amount = amount;
}
