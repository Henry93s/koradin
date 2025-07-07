#ifndef INFO_H
#define INFO_H

#include <QString>

class Info {
    enum InfoType{
        User,
        Product,
        Music,
        None
    };

public:
    Info();
    ~Info();

    QString getName();
    void setName(const QString& name);

    QString GetID();
    void SetID(const QString& ID);
private:
    QString name;
    QString ID;
};

#endif // INFO_H
