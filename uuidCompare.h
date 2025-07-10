#ifndef UUIDCOMPARE_H
#define UUIDCOMPARE_H
#include <QString>

template <typename T>
static bool uuidIsduplicate(const T& list, const QString& uuid){
    for(auto it : list){
        if(it->getUuid().compare(uuid) == 0){
            return true;
            break;
        }
    }
    return false;
}

#endif // UUIDCOMPARE_H
