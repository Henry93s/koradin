#include "userInfo.h"

UserInfo::UserInfo(){}
UserInfo::UserInfo(const QString& ID, const QString& name, const QString& password
                   , const QString& email, const QString& isAdmin, const QString& salt){
    this->setID(ID);
    this->setName(name);
    this->setPassword(password);
    this->setEmail(email);
    this->setIsAdmin(isAdmin);
    this->setSalt(salt);
}
UserInfo::~UserInfo(){}
const QString& UserInfo::getID() const{
    return this->ID;
}
void UserInfo::setID(const QString& ID){
    this->ID = ID;
}
QString UserInfo::getSalt(){
    return this->salt;
}
void UserInfo::setSalt(const QString& salt){
    this->salt = salt;
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
