#include "userManager.h"

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMap>
#include <QString>
#include <QDebug>
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "userInfo.h"
// windows, mac 다른 파일 경로 고려하여 filepath 수정 (250706)
#include <QCoreApplication>
#include <QDir>

using namespace std;
using json = nlohmann::json;

// singleton 인스턴스 초기화
UserManager* UserManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
UserManager* UserManager::getInstance(){
    if(!instance){
        instance = new UserManager();
    }
    return instance;
}

void UserManager::userListJsonLoad(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../userList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../userList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "userList 파일 오픈 실패";
        return;
    }

    QTextStream in(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString jsonText = in.readAll();

    // Qstring -> standard string
    string jsonStdStr = jsonText.toStdString();

    json j = json::parse(jsonStdStr);

    for(auto user : j){
        QString ID = QString::fromStdString(user["ID"]);
        QString name = QString::fromStdString(user["name"]);
        QString password = QString::fromStdString(user["password"]);
        QString email = QString::fromStdString(user["email"]);
        QString isAdmin = QString::fromStdString(user["isAdmin"]);
        QString salt = QString::fromStdString(user["salt"]);

        UserInfo* newUser = new UserInfo(ID, name, password, email, isAdmin, salt);
        // userList stl Map 컨테이너에 저장
        userList.insert(ID, newUser);

        qDebug() << "Loaded Member ID, name : " << ID << " (" << name << ")";
    }

    file.close();
}
void UserManager::userListJsonSave(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../userList.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../userList.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "userList 파일 저장 실패";
        return;
    }

    QTextStream out(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    out.setEncoding(QStringConverter::Utf8);

    json j_array = json::array();  // 최상위 배열 구조

    for (auto it = userList.begin(); it != userList.end(); ++it) {
        if (!it.value()) continue;
        json userObj = {
            { "ID",        it.value()->getID().toStdString() },
            { "name",      it.value()->getName().toStdString() },
            { "password",  it.value()->getPassword().toStdString() },
            { "email",     it.value()->getEmail().toStdString() },
            { "isAdmin",   it.value()->getIsAdmin().toStdString() },
            { "salt",      it.value()->getSalt().toStdString() }
        };
        // json 각 요소들을 j_array 에 push
        j_array.push_back(userObj);
    }

    // JSON 텍스트 생성
    QString jsonText = QString::fromStdString(j_array.dump(4)); // 4칸 들여쓰기 pretty print
    out << jsonText;

    file.close();
    qDebug() << "userList 저장 완료";
}

std::vector<UserInfo *> UserManager::getUserListAsVector() const
{
    std::vector<UserInfo*> result;
    result.reserve(userList.size());

    for (auto it = userList.begin(); it != userList.end(); ++it) {
        result.push_back(it.value());
    }

    return result;
}

// create user
QString UserManager::userInsert(UserInfo* user){
    for (auto it = userList.begin(); it != userList.end(); it++) {
        if (!it.value()) continue;
        if(user->getID().compare(it.value()->getID()) == 0){
            // 중복 아이디 처리
            return "Duplicate_ID";
        }
    }

    // 아이디 중복되지 않음
    userList.insert(user->getID(), user);

    return "OK";
}

// delete user
QString UserManager::userErase(const QString& ID){
    for (auto it = userList.begin(); it != userList.end();) {
        if (!it.value()) continue;
        if(ID.compare(it.value()->getID()) == 0){
            userList.erase(it);
            return "OK";
        } else {
            it++;
        }
    }

    return "NotFound";
}

// read one user
UserInfo* UserManager::userSearchById(const QString& ID){
    UserInfo* returnUser;

    for (auto it = userList.begin(); it != userList.end(); ++it) {
        if (!it.value()) continue;
        if(ID.compare(it.value()->getID()) == 0){
            returnUser = it.value();
            return returnUser;
        }
    }

    return nullptr;
}

// read userList
QMap<QString, UserInfo*> UserManager::userListRead(){
    return this->userList;
}

// read some users search by ID
QMap<QString, UserInfo*> UserManager::userSearchAllById(const QString& ID){
    QMap<QString, UserInfo*> returnUsers;
    for (auto it = userList.begin(); it != userList.end(); ++it) {
        if (!it.value()) continue;
        if(it.value()->getID().contains(ID) == true){
            returnUsers.insert(it.value()->getName(), it.value());
        }
    }
    return returnUsers;
}
// read some users search by name
QMap<QString, UserInfo*> UserManager::userSearchAllByName(const QString& name){
    QMap<QString, UserInfo*> returnUsers;
    for (auto it = userList.begin(); it != userList.end(); ++it) {
        if (!it.value()) continue;
        if(it.value()->getName().contains(name) == true){
            returnUsers.insert(it.value()->getName(), it.value());
        }
    }
    return returnUsers;
}

UserManager::UserManager()
{
    userListJsonLoad();
}

UserManager::~UserManager()
{
    qDebug() << "프로그램 종료 발생";
    userListJsonSave();
}
