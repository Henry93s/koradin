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

using namespace std;
using json = nlohmann::json;

// singleton 인스턴스 초기화
UserManager* UserManager::instance = nullptr;

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
UserManager* UserManager::getInstance(){
    if(!(this->instance)){
        this->instance = new UserManager();
    }
    return this->instance;
}

void UserManager::userListJsonLoad(){
    // Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    QFile file("./../../userList.json");
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

        UserInfo* newUser = new UserInfo(ID, name, password, email, isAdmin);
        // userList stl Map 컨테이너에 저장
        userList.insert(ID, newUser);

        qDebug() << "Loaded Member ID, name : " << ID << " (" << name << ")\n";
    }

    file.close();
}
void UserManager::userListJsonSave(){
    QFile file("./../../userList.json");
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
            { "isAdmin",   it.value()->getIsAdmin().toStdString() }
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

UserManager::UserManager()
{
    userListJsonLoad();
}

UserManager::~UserManager()
{
    userListJsonSave();
}
