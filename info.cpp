#include "info.h"

Info::Info(){}
Info::~Info(){}


QString Info::getName(){
    return this->name;
}
void Info::setName(const QString& productName){
    this->name = productName;
}
