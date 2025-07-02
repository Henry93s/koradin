#include "info.h"

Info::Info(){}
Info::~Info(){}


QStringView Info::getName(){
    return this->name;
}
void Info::setName(QString& productName){
    this->name = productName;
}
