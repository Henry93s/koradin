#include "bluerayManager.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDebug>
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "blueray.h"
#include <QApplication>
#include <QDir>

using namespace std;
using json = nlohmann::json;

// singleton 인스턴스 초기화
BluerayManager* BluerayManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
BluerayManager* BluerayManager::getInstance(){
    if(!instance){
        instance = new BluerayManager();
    }
    return instance;
}

void BluerayManager::bluerayListJsonLoad(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../bluerayList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../bluerayList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "bluerayList 파일 오픈 실패";
        return;
    }

    QTextStream in(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString jsonText = in.readAll();

    // Qstring -> standard string
    string jsonStdStr = jsonText.toStdString();

    json j = json::parse(jsonStdStr);

    for(auto blueray : j){
        QString name = QString::fromStdString(blueray["name"]);
        QString artist = QString::fromStdString(blueray["artist"]);
        QString company = QString::fromStdString(blueray["company"]);
        int price = blueray["price"];
        QString context = QString::fromStdString(blueray["context"]);
        int amount = blueray["amount"];

        Blueray* newBlueray = new Blueray(name, artist, company, price, context, amount);
        // bluerayList stl Map 컨테이너에 저장
        bluerayList.insert(name, newBlueray);

        qDebug() << "Loaded Blueray name, artist : " << name << " (" << artist << ")";
    }

    file.close();
}
void BluerayManager::bluerayListJsonSave(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../bluerayList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../bluerayList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "bluerayList 파일 저장 실패";
        return;
    }

    QTextStream out(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    out.setEncoding(QStringConverter::Utf8);

    json j_array = json::array();  // 최상위 배열 구조

    for (auto it = bluerayList.begin(); it != bluerayList.end(); ++it) {
        if (!it.value()) continue;
        json userObj = {
            { "name",        it.value()->getName().toStdString() },
            { "artist",      it.value()->getArtist().toStdString() },
            { "company",  it.value()->getCompany().toStdString() },
            { "price",     it.value()->getPrice() },
            { "context",   it.value()->getContext().toStdString() },
            { "amount",    it.value()->getAmount() }
        };
        // json 각 요소들을 j_array 에 push
        j_array.push_back(userObj);
    }

    // JSON 텍스트 생성
    QString jsonText = QString::fromStdString(j_array.dump(4)); // 4칸 들여쓰기 pretty print
    out << jsonText;

    file.close();
    qDebug() << "bluerayList 저장 완료";
}

// create blueray
QString BluerayManager::bluerayInsert(Blueray* blueray){
    for (auto it = bluerayList.begin(); it != bluerayList.end(); it++) {
        if (!it.value()) continue;
        if(blueray->getName().compare(it.value()->getName()) == 0
            && blueray->getArtist().compare(it.value()->getArtist()) == 0){
            // 중복 blueray 처리
            return "Duplicate_MUSIC";
        }
    }

    // blueray 중복되지 않음
    bluerayList.insert(blueray->getName(), blueray);

    return "OK";
}

// delete blueray
QString BluerayManager::bluerayErase(const QString& name, const QString& artist){
    for (auto it = bluerayList.begin(); it != bluerayList.end();) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && artist.compare(it.value()->getArtist()) == 0){
            bluerayList.erase(it);
            return "OK";
        } else {
            it++;
        }
    }

    return "NotFound";
}

// read one blueray
Blueray* BluerayManager::blueraySearchByNameAndArtist(const QString& name, const QString& artist){
    Blueray* returnBlueray;

    for (auto it = bluerayList.begin(); it != bluerayList.end(); ++it) {
        if (!it.value()) continue;
        if(name.compare(it.value()->getName()) == 0
            && artist.compare(it.value()->getArtist()) == 0){
            returnBlueray = it.value();
            return returnBlueray;
        }
    }

    return nullptr;
}

// read some bluerays search by name
QMap<QString, Blueray*> BluerayManager::blueraySearchAllByName(const QString& name){
    QMap<QString, Blueray*> returnBluerays;

    for(auto it = bluerayList.begin(); it != bluerayList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getName().contains(name) == true){
            returnBluerays.insert(it.value()->getName(), it.value());
        }
    }

    return returnBluerays;
}

// read some bluerays search by artist
QMap<QString, Blueray*> BluerayManager::blueraySearchAllByArtist(const QString& artist){
    QMap<QString, Blueray*> returnBluerays;

    for(auto it = bluerayList.begin(); it != bluerayList.end(); ++it){
        if(!it.value()) continue;
        if(it.value()->getArtist().contains(artist) == true){
            returnBluerays.insert(it.value()->getName(), it.value());
        }
    }

    return returnBluerays;
}

// read bluerayList
QMap<QString, Blueray*> BluerayManager::bluerayListRead(){
    return this->bluerayList;
}

BluerayManager::BluerayManager()
{
    bluerayListJsonLoad();
}

BluerayManager::~BluerayManager()
{
    qDebug() << "프로그램 종료 발생";
    bluerayListJsonSave();
}
