#include "userInfo.h"

UserInfo::UserInfo(){}
UserInfo::UserInfo(const QString& ID, const QString& name, const QString& password
                   , const QString& email, const QString& isAdmin){
    this->setID(ID);
    this->setName(name);
    this->setPassword(password);
    this->setEmail(email);
    this->setIsAdmin(isAdmin);
}
UserInfo::~UserInfo(){}
QString UserInfo::getID(){
    return this->ID;
}
void UserInfo::setID(const QString& ID){
    this->ID = ID;
}
QString UserInfo::getPassword(){
    return this->password;
}
void UserInfo::setPassword(const QString& password){
    this->password = password;
}
QString UserInfo::getEmail(){
    return this->email;
}
void UserInfo::setEmail(const QString& email){
    this->email = email;
}
QString UserInfo::getIsAdmin(){
    return this->isAdmin;
}
void UserInfo::setIsAdmin(const QString& isAdmin){
    this->isAdmin = isAdmin;
}
