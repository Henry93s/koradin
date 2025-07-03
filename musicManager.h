#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "mainController.h"
#include "music.h"
#include <QVector>
#include <QMap>

class MusicManager : public MainController {

public:
    MusicManager();
    ~MusicManager();

    // STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
    MusicManager* getInstance();

    // create music
    QString musicInsert(Music* music);
    // delete music
    QString musicErase(const QString& name, const QString& artist);
    // read one music
    Music* musicSearchByNameAndArtist(const QString& name, const QString& artist);
    // read musicListSt
    QMap<QString, Music*> musicListRead();
    // read some musics (검색 데이터 포함 조회)
    QMap<QString, Music*> musicSearchAllByNameAndArtist(const QString& name, const QString& artist);

    // musicList.json 파일 -> musicList 로 load
    void musicListJsonLoad();
    // QMap musicList -> musicList.json 파일로 save
    void musicListJsonSave();

private:
    static MusicManager* instance;
    QMap<QString, Music*> musicList;
};


#endif // MUSICMANAGER_H
