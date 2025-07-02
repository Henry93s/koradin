#include "mainController.h"

MainController::MainController(){}
MainController::~MainController(){}

// STL file 을 관리하기 때문에, 서버에서 한 번 인스턴스 생성을 위한 싱글턴 패턴 적용
MainController* MainController::getInstance(){
    if(!(this->instance)){
        this->instance = new MainController();
    }
    return this->instance;
}
