#ifndef INFO_H
#define INFO_H

#include <QString>

class Info {
public:
    Info();
    ~Info();

    QString getName();
    void setName(const QString& name);

private:
    QString name;

};

#endif // INFO_H
