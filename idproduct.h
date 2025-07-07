#ifndef IDPRODUCT_H
#define IDPRODUCT_H

#include <QJsonArray>
#include <QJsonObject>

static bool ThereIsId(const QString& newId, const QJsonArray& jsonArray){
    for(auto& json : jsonArray){
        QJsonObject obj = json.toObject();
        if(obj["ID"] == newId){
            return true;
        }
    }
    return false;
}

static QString GetId(const QJsonArray& jsonArray){
    QString newId;
    do {
        newId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    } while (ThereIsId(newId, jsonArray));

    return newId;
}

#endif // IDPRODUCT_H
