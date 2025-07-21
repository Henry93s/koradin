#ifndef LOGMANAGER_H
#define LOGMANAGER_H
#include "mainController.h"
#include <QFile>
#include <QTextStream>



class LogManager : public MainController
{
public:
    enum LogType{
        INFO,
        WARN,
        ERRO,
        DEBUG
    };

    LogManager();
    ~LogManager();

    void getTimeStamp_and_write(LogManager::LogType logtype, const QString& msg);

    static LogManager* getInstance();
private:
    static LogManager* instance;
    QFile* log;
    QTextStream* out;

};

#endif // LOGMANAGER_H
