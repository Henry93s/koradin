#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "join.h"

#include <QMessageBox>
#include <QFile>
#include <QString>
#include <string>
#include <QTabWidget>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "server.h"
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "popup.h"
#include <QApplication>
#include <QDir>

using namespace std;
using json = nlohmann::json;

int now_translation_index = 0; // default : 0 (한국어)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    // 프로그램 처음 켜질 때 user, music manager getInstance 호출
    this->usermanager = this->usermanager->getInstance();

    //connect(ui->admin_button, &QPushButton::clicked, this, &MainWindow::on_code_Typed);
    this->musicmanager = this->musicmanager->getInstance();
    this->bluerayManager = this->bluerayManager->getInstance();
    this->bookManager = this->bookManager->getInstance();

    // 셋팅된 언어 변환 콤보박스의 인덱스 적용
    ui->language_comboBox->setCurrentIndex(now_translation_index);

    // server 가 open 됨을 확인하고, 로그인 검증이 통과될 경우 현재 mainwindow를 닫고 client.ui 를 연다.
    // 그 다음 소켓도 넘겨준다.
    // server connect 를 통한 새 클라이언트 연결 추가.
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", PORT);

    // server 가 open 됨을 확인
    // connect(socket, &QTcpSocket::errorOccurred, [=](QAbstractSocket::SocketError) {
    //     QMessageBox::information(this, tr("Communication"), tr("서버 안 열림!"));
    //     socket->close();
    // });

    connect(socket, &QTcpSocket::connected, [=]() {
        connect(socket, SIGNAL(readyRead()), SLOT(respond()));
        QMessageBox::information(this, tr("Communication"), tr("서버 연결!\n클라이언트로 접속하십시오!"));
    });
}

MainWindow::~MainWindow()
{
    // main 프로그램 종료 시 user,music STL 컨테이너 데이터를 ~list.json 에 저장 하도록 함
    delete this->usermanager;
    delete this->musicmanager;
    delete this->bluerayManager;
    delete this->bookManager;
    delete ui;
}

void MainWindow::on_login_button_clicked()
{
    // 로그인 검증
    // ID 검증
    QJsonObject obj;
    QJsonObject data;
    obj["CommuType"] = tr("AUTH");
    data["ID"] = ui->id_lineEdit->text();
    data["password"] = ui->pw_lineEdit->text();
    obj["Data"] = data;

    QJsonDocument doc(obj);
    socket->write(doc.toJson(QJsonDocument::Compact));

    // UserInfo* temp = usermanager->userSearchById(ui->id_lineEdit->text());
    // if(temp == nullptr){
    //     // id 불일치
    //     qDebug() << "입력한 id 가 유저 리스트에 없습니다.";
    //     Popup* popup = new Popup(this, tr("존재하지 않는 아이디 입니다."));
    //     popup->show();
    // } else{
    //     // password 검증
    //     if(ui->pw_lineEdit->text().compare(temp->getPassword()) == 0){
    //         // client.ui 가 열렸을 때, server connect 를 통한 새 클라이언트 연결 추가.
    //         // commuInfo -> client session 관리 리스트에 세션 추가

    //         // client ui open
    //         Client* clientWindow = new Client();
    //         clientWindow->show();

    //         this->close();
    //     } else {
    //         // password 불일치
    //         qDebug() << "계정의 password 가 일치하지 않습니다.";
    //         Popup* popup = new Popup(this, tr("계정의 password 가 일치하지 않습니다."));
    //         popup->show();
    //     }
    // }
}


void MainWindow::on_join_button_clicked()
{
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
    // 회원 가입 버튼 클릭 시 열리는 회원 가입 화면
    Join* joinWindow = new Join(); // 다시 돌아오기 위해서 Join 생성자에 현재 window 를 인자로 넘김
    joinWindow->Initialize(socket); // 소켓을 넘김. 확인하기 위해서
    disconnect(socket, SIGNAL(readyRead()), this, SLOT(respond()));
    joinWindow->show();

    // 회원 가입 취소 또는 완료 후 다시 로그인 화면으로 돌아옴
    this->close();
}

QString MainWindow::managerKeyJsonLoad(){
    QString runFilePath = QCoreApplication::applicationDirPath();
    QString jsonPath;
#ifdef Q_OS_WIN
    jsonPath = QDir(runFilePath).filePath("../../../managerKey.json");
#elif defined(Q_OS_MAC)
    jsonPath = QDir(runFilePath).filePath("../../../../../managerKey.json");
#endif

    // win : Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    // mac : ......debug > app > contents > MacOS 안에 실행 파일이 있음
    QFile file(jsonPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "managerKey 파일 오픈 실패";
        return "managerKey 파일 오픈 실패";
    }

    QTextStream in(&file);
    // json 에 한글 포함일 때 encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString jsonText = in.readAll();

    // Qstring -> standard string
    string jsonStdStr = jsonText.toStdString();

    json j = json::parse(jsonStdStr);

    QString key;
    for(auto data : j){
        key = QString::fromStdString(data["key"]);
        qDebug() << "Loaded key : " << key;
    }

    file.close();
    return key;
}

void MainWindow::respond()
{
    QTcpSocket* clientSocket = dynamic_cast<QTcpSocket*>(sender());
    if(clientSocket->bytesAvailable() > BLOCK_SIZE) return;
    QByteArray bytearray = clientSocket->read(BLOCK_SIZE);

    //commuInfoQueue.push(CommuInfo{bytearray});
    auto info = CommuInfo{bytearray};

    auto type = info.GetType();

    if(type == CommuType::AUTH){
        auto auth = info.GetIDPwd();
        if(auth.first != QString("No") && auth.second != QString("No")){
            disconnect(socket, SIGNAL(readyRead()), this, SLOT(respond()));
            // client ui open
            Client* clientWindow = new Client();
            clientWindow->Initialize(socket, auth.first);
            clientWindow->show();


            this->close();
        }
        else if(auth.first == QString("No")){
            // id 불일치
            qDebug() << "입력한 id 가 유저 리스트에 없습니다.";
            // Popup* popup = new Popup(this, tr("존재하지 않는 아이디 입니다."));
            // popup->show();
        }
        else{
            // password 불일치
            qDebug() << "계정의 password 가 일치하지 않습니다.";
            Popup* popup = new Popup(this, tr("계정의 password 가 일치하지 않습니다."));
            popup->show();
        }
    }
}

void MainWindow::on_admin_button_clicked()
{
    if(ui->admin_lineEdit->text().compare(managerKeyJsonLoad()) != 0){
        qDebug() << "관리자 코드가 일치하지 않습니다.";
        Popup* popup = new Popup(this, tr("관리자 코드가 일치하지 않습니다."));
        popup->show();
    } else {
        qDebug() << "관리자 코드 일치 확인";

        // 서버 진입
        Server* server = new Server();
        server->show();
        this->close();
    }
}


void MainWindow::on_language_comboBox_activated(int index)
{
    QString langCode;

    // 현재 적용된 translator 를 제거하고 콤보박스로 적용된 translator 를 적용함
    if (translator) {
        qApp->removeTranslator(translator);
        delete translator;
        translator = nullptr;
    }

    translator = new QTranslator();

    if (index == 0) {
        now_translation_index = index;
    } else if (index == 1) {
        now_translation_index = index;
    }

    if(now_translation_index == 0){
        // Korean 한국어 - default
        langCode = "ko_KR";
    } else if(now_translation_index == 1){
        // English
        langCode = "en_US";
    }

    QString runFilePath = QCoreApplication::applicationDirPath();
    QString filename;
#ifdef Q_OS_WIN
    filename = QDir(runFilePath).filePath("./Koradin_" + langCode + ".qm");
#elif defined(Q_OS_MAC)
    filename = QDir(runFilePath).filePath("../../../.qm/Koradin_" + langCode + ".qm");
#endif

    if (translator->load(filename)) {
        qApp->installTranslator(translator);
        // 새 언어에 맞게 UI 갱신
        ui->retranslateUi(this);
    } else {
        qDebug() << "Failed to load translation file:" << filename;
    }
}

void MainWindow::on_code_Typed()
{
    // Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    QFile file("./../../CODE");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "CODE 파일 오픈 실패";
        QMessageBox::critical(this, tr("file"), tr("연결 실패!"));
        return;
    }

    QTextStream in(&file);
    // encoding 셋팅 필요
    in.setEncoding(QStringConverter::Utf8);
    QString code = in.readAll();

    // 입력 코드가 맞음
    if(ui->admin_lineEdit->text() == code){
        //서버 나타냄
        Server* server = new Server();
        server->show();

        //이건 꺼짐
        this->close();
    } else{
        //그저 메시지박스
        QMessageBox::critical(this, tr("Echo Server"), tr("code Unmatched!"));
    }
}

