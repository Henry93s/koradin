#ifndef BLUERAYMANAGER_H
#define BLUERAYMANAGER_H

#include "mainController.h"
#include "blueray.h"
#include <QVector>
#include <QMap>

class BluerayManager : public MainController {

public:
    BluerayManager();
    ~BluerayManager();

    // STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
    static BluerayManager* getInstance();

    // create blueray
    QString bluerayInsert(Blueray* blueray);
    // delete blueray
    QString bluerayErase(const QString& name, const QString& artist);
    // read one blueray
    Blueray* blueraySearchByNameAndArtist(const QString& name, const QString& artist);
    // read bluerayList
    QMap<QString, Blueray*> bluerayListRead();
    // read some bluerays search by name (검색 데이터 포함 조회)
    QMap<QString, Blueray*> blueraySearchAllByName(const QString& name);
    // read some bluerays search by artist
    QMap<QString, Blueray*> blueraySearchAllByArtist(const QString& artist);
    // read one blueray search by Uuid
    Blueray* blueraySearchByUuid(const QString& uuid);
    QString bluerayEraseUuid(const QString& uuid);

    // bluerayList.json 파일 -> bluerayList 로 load
    void bluerayListJsonLoad();
    // QMap bluerayList -> bluerayList.json 파일로 save
    void bluerayListJsonSave();

private:
    static BluerayManager* instance;
    QMap<QString, Blueray*> bluerayList;
};


#endif // BLUERAYMANAGER_H
