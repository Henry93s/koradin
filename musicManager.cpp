#include "musicManager.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDebug>
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "music.h"
#include <QApplication>
#include <QDir>

using namespace std;
using json = nlohmann::json;

// singleton 인스턴스 초기화
MusicManager* MusicManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
MusicManager* MusicManager::getInstance(){
    if(!instance){
        instance = new MusicManager();
    }
    return instance;
}

void MusicManager::musicListJsonLoad(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../musicList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../musicList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "musicList 파일 오픈 실패";
        return;
    }

    QTextStream in(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString jsonText = in.readAll();

    // Qstring -> standard string
    string jsonStdStr = jsonText.toStdString();

    json j = json::parse(jsonStdStr);

    for(auto music : j){
        QString name = QString::fromStdString(music["name"]);
        QString artist = QString::fromStdString(music["artist"]);
        QString company = QString::fromStdString(music["company"]);
        int price = music["price"];
        QString context = QString::fromStdString(music["context"]);
        int amount = music["amount"];
        QString image = QString::fromStdString(music["image"]);

        Music* newMusic = new Music(name, artist, company, price, context, amount, image);
        // musicList stl Map 컨테이너에 저장
        musicList.insert(name, newMusic);

        qDebug() << "Loaded Music name, artist : " << name << " (" << artist << ")";
    }

    file.close();
}
void MusicManager::musicListJsonSave(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../musicList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../musicList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "musicList 파일 저장 실패";
        return;
    }

    QTextStream out(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    out.setEncoding(QStringConverter::Utf8);

    json j_array = json::array();  // 최상위 배열 구조

    for (auto it = musicList.begin(); it != musicList.end(); ++it) {
        if (!it.value()) continue;
        json userObj = {
            { "name",        it.value()->getName().toStdString() },
            { "artist",      it.value()->getArtist().toStdString() },
            { "company",  it.value()->getCompany().toStdString() },
            { "price",     it.value()->getPrice() },
            { "context",   it.value()->getContext().toStdString() },
            { "amount",    it.value()->getAmount() },
            { "image",     it.value()->getQImagePureQString().toStdString()}
        };
        // json 각 요소들을 j_array 에 push
        j_array.push_back(userObj);
    }

    // JSON 텍스트 생성
    QString jsonText = QString::fromStdString(j_array.dump(4)); // 4칸 들여쓰기 pretty print
    out << jsonText;

    file.close();
    qDebug() << "musicList 저장 완료";
}

// create music
QString MusicManager::musicInsert(Music* music){
    for (auto it = musicList.begin(); it != musicList.end(); it++) {
        if (!it.value()) continue;
        if(music->getName().compare(it.value()->getName()) == 0
            && music->getArtist().compare(it.value()->getArtist()) == 0){
            // 중복 music 처리
            return "Duplicate_MUSIC";
        }
    }

    // music 중복되지 않음
    musicList.insert(music->getName(), music);

    return "OK";
}

// delete music
QString MusicManager::musicErase(const QString& name, const QString& artist){
    for (auto it = musicList.begin(); it != musicList.end();) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && artist.compare(it.value()->getArtist()) == 0){
            musicList.erase(it);
            return "OK";
        } else {
            it++;
        }
    }

    return "NotFound";
}

// read one music
Music* MusicManager::musicSearchByNameAndArtist(const QString& name, const QString& artist){
    Music* returnMusic;

    for (auto it = musicList.begin(); it != musicList.end(); ++it) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && artist.compare(it.value()->getArtist()) == 0){
            returnMusic = it.value();
            return returnMusic;
        }
    }

    return nullptr;
}

// read musicList
QMap<QString, Music*> MusicManager::musicListRead(){
    return this->musicList;
}

// read some musics Search by name
QMap<QString, Music*> MusicManager::musicSearchAllByName(const QString& name){
    QMap<QString, Music*> returnMusics;

    for(auto it = musicList.begin(); it != musicList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getName().contains(name) == true){
            returnMusics.insert(it.value()->getName(), it.value());
        }
    }

    return returnMusics;
}

// read some musics search by artist
QMap<QString, Music*> MusicManager::musicSearchAllByArtist(const QString& artist){
    QMap<QString, Music*> returnMusics;

    for(auto it = musicList.begin(); it != musicList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getArtist().contains(artist) == true){
            returnMusics.insert(it.value()->getName(), it.value());
        }
    }

    return returnMusics;
}

MusicManager::MusicManager()
{
// test data insert
/*
    QImage img("./../../music_images/From Our 20s.PNG");

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    img.save(&buffer, "PNG");

    QString img_string = QString::fromLatin1(byteArray.toBase64());

    // test
    Music* music = new Music("미니 6집 From Our 20's", "프로미스나인", "드림어스컴퍼니", 23000, "프로미스나인(fromis_9)의 여섯 번째 미니앨범인 From Our 20's는 20대의 감정과 순간들을 담아낸 앨범입니다. 타이틀곡 LIKE YOU BETTER를 포함해 총 6곡이 수록되어 있으며, 멤버 5인 체제로 재편된 후 처음으로 발매하는 앨범입니다. ", 100, img_string);
    this->musicInsert(music);
    // test end
*/
    musicListJsonLoad();
}

MusicManager::~MusicManager()
{
    qDebug() << "프로그램 종료 발생";
    musicListJsonSave();
}
