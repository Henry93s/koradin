#ifndef USERINFO_H
#define USERINFO_H

#include "info.h"
#include <QString>

class UserInfo : public Info{

public:
    UserInfo();
    ~UserInfo();
    QStringView getID();
    void setID(QString& ID);
    QStringView getPassword();
    void setPassword(QString& password);
    QStringView getEmail();
    void setEmail(QString& email);
    bool getIsAdmin();
    void setIsAdmin(bool isAdmin);

private:
    QString ID;
    QString password;
    QString email;
    bool isAdmin;
};

#endif // USERINFO_H
