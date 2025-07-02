#include "userInfo.h"

UserInfo::UserInfo(){}
UserInfo::~UserInfo(){}
QStringView UserInfo::getID(){
    return this->ID;
}
void UserInfo::setID(QString& ID){
    this->ID = ID;
}
QStringView UserInfo::getPassword(){
    return this->password;
}
void UserInfo::setPassword(QString& password){
    this->password = password;
}
QStringView UserInfo::getEmail(){
    return this->email;
}
void UserInfo::setEmail(QString& email){
    this->email = email;
}
bool UserInfo::getIsAdmin(){
    return this->isAdmin;
}
void UserInfo::setIsAdmin(bool isAdmin){
    this->isAdmin = isAdmin;
}
