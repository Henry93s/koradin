#ifndef USERINFO_H
#define USERINFO_H

#include "info.h"
#include <QString>

class UserInfo : public Info{

public:
    UserInfo();
    UserInfo(const QString& ID, const QString& name, const QString& password
             , const QString& email, const QString& isAdmin, const QString& salt);
    ~UserInfo();
    const QString& getID() const; // primary Key
    void setID(const QString& ID);
    QString getPassword();
    void setPassword(const QString& password);
    QString getEmail();
    void setEmail(const QString& email);
    QString getIsAdmin();
    void setIsAdmin(const QString& isAdmin);
    QString getSalt();
    void setSalt(const QString& salt);

private:
    QString ID;
    QString password;
    QString email;
    QString isAdmin;
    QString salt;
};

#endif // USERINFO_H
