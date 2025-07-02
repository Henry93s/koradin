#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "mainController.h"

class userManager : public MainController {
public:
    // STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
    userManager* getInstance();

private:
    static userManager* instance;
};


#endif // USERMANAGER_H
