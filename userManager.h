#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "mainController.h"
#include "userInfo.h"
#include <QVector>
#include <QMap>

class UserManager : public MainController {

public:
    UserManager();
    ~UserManager();

    // STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
    UserManager* getInstance();

    // create user
    void userListInsert(UserInfo* user);
    // delete user
    void userListErase(const QString& ID);
    // read one user
    UserInfo* userListRead(const QString& ID);
    // read userList
    QMap<QString, QVector<UserInfo*>> userListRead();

    // userList.json 파일 -> userList 로 load
    void userListJsonLoad();
    // QMap userList -> userList.json 파일로 save
    void userListJsonSave();

private:
    static UserManager* instance;
    // userList.json 파일을 읽어서 프로그램에서는 userList STL 컨테이너로 user 데이터 관리
    // 소멸하거나 필요 시 json 파일에 다시 저장함
    QMap<QString, QVector<UserInfo*>> userList;

};


#endif // USERMANAGER_H
