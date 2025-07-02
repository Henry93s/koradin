#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

class MainController {
public:
    MainController();
    ~MainController();

public:
    MainController* getInstance();
private:
    MainController* instance = nullptr;
};

#endif // MAINCONTROLLER_H
