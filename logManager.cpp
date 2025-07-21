#include "logManager.h"
#include "time.h"
#include <QApplication>
#include <QDir>
#include <QString>

// singleton 인스턴스 초기화
LogManager* LogManager::instance = nullptr;

LogManager* LogManager::getInstance(){
    if(!instance){
        instance = new LogManager();
    }
    return instance;
}

LogManager::LogManager() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char logFile[256];

    QString runFilePath = QCoreApplication::applicationDirPath();
    QString logPath;
#ifdef Q_OS_WIN
    QDir Dir(QDir(runFilePath).filePath("../../../logs"));
    if(!Dir.exists()){
        Dir.mkdir(QDir(runFilePath).filePath("../../../logs"));
        qDebug() << "logs 디렉토리 생성";
    }

    // 날짜별 로그 파일 생성을 위함
    snprintf(logFile, sizeof(logFile), "../../../logs/koradin_%04d%02d%02d.log"
             , t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
#elif defined(Q_OS_MAC)
    QDir Dir(QDir(runFilePath).filePath("../../../../../logs"));
    if(!Dir.exists()){
        Dir.mkdir(QDir(runFilePath).filePath("../../../../../logs"));
        qDebug() << "logs 디렉토리 생성";
    }

    // 날짜별 로그 파일 생성을 위함
    snprintf(logFile, sizeof(logFile), "../../../../../logs/koradin_%04d%02d%02d.log"
             , t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
#endif
    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    // 로그 파일 열기
    this->log = new QFile(QDir(runFilePath).filePath(logFile));

    if(!log->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)){
        qDebug() << "로그 파일 open 실패";
        return;
    }
    out = new QTextStream(log);
    out->setEncoding(QStringConverter::Utf8);
}

LogManager::~LogManager(){
    this->log->close();
    delete this->log;
    delete this->out;
}


void LogManager::getTimeStamp_and_write(LogManager::LogType logtype, const QString& msg){
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", t);

    QString writeMsg;
    QString writeType;
    switch (logtype) {
    case LogManager::LogType::INFO:
        writeType = "[INFO] : ";
        break;
    case LogManager::LogType::DEBUG:
        writeType = "[DEBUG] : ";
        break;
    case LogManager::LogType::ERRO:
        writeType = "[ERRO] : ";
        break;
    case LogManager::LogType::WARN:
        writeType = "[WARN] : ";
        break;
    default:
        break;
    }
    writeMsg = QString("%1%2%3").arg(timestamp, writeType, msg);
    *out << writeMsg << "\n";
    out->flush();
}
