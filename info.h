#ifndef INFO_H
#define INFO_H

#include <QString>

class Info {
public:
    Info();
    ~Info();

    QStringView getName();
    void setName(QString& name);

private:
    QString name;

};

#endif // INFO_H
