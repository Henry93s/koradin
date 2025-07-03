#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "join.h"
#include <QFile>
#include <QString>
#include <string>
#include <QTabWidget>
#include <QDebug>
#include "server.h"
// json 파싱 라이브러리(json.hpp - by https://github.com/nlohmann/json/releases/tag/v3.12.0) 적용
#include "json.hpp"
#include "popup.h"
using namespace std;
using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    // usermanager getInstance 호출(로그인 검증 로직 과 회원 가입 처리 로직 시 필요함)
    this->usermanager = this->usermanager->getInstance();
}

MainWindow::~MainWindow()
{
    // main 프로그램 종료 시 현재 userlist 를 userlist.json 에 저장 하도록 하는 함수 호출됨
    delete this->usermanager;
    delete ui;
}

void MainWindow::on_login_button_clicked()
{
    // server 가 open 됨을 확인

    // 로그인 검증
    // ID 검증
    UserInfo* temp = usermanager->userSearchById(ui->id_lineEdit->text());
    if(temp == nullptr){
        // id 불일치
        qDebug() << "입력한 id 가 유저 리스트에 없습니다.\n";
        Popup* popup = new Popup(this, "존재하지 않는 아이디 입니다.");
        popup->show();
    } else{
        // password 검증
        if(ui->pw_lineEdit->text().compare(temp->getPassword()) == 0){
            // client.ui 가 열렸을 때, server connect 를 통한 새 클라이언트 연결 추가.
            // commuInfo -> client session 관리 리스트에 세션 추가

            // client ui open
            Client* clientWindow = new Client();
            clientWindow->show();

            this->close();
        } else {
            // password 불일치
            qDebug() << "계정의 password 가 일치하지 않습니다.\n";
            Popup* popup = new Popup(this, "계정의 password 가 일치하지 않습니다.");
            popup->show();
        }
    }
}


void MainWindow::on_join_button_clicked()
{
    // 회원 가입 버튼 클릭 시 열리는 회원 가입 화면
    Join* joinWindow = new Join(); // 다시 돌아오기 위해서 Join 생성자에 현재 window 를 인자로 넘김
    joinWindow->show();

    // 회원 가입 취소 또는 완료 후 다시 로그인 화면으로 돌아옴
    this->close();
}

QString MainWindow::managerKeyJsonLoad(){
    // Qt creator 에서는 기본적으로 파일을 열 때 프로젝트명~debug or 프로젝트명~release 디렉토리에서 실행되므로 현재 파일 위치로 변경
    QFile file("./../../managerKey.json");
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
        qDebug() << "Loaded key : " << key << "\n";
    }

    file.close();
    return key;
}

void MainWindow::on_admin_button_clicked()
{
    if(ui->admin_lineEdit->text().compare(managerKeyJsonLoad()) != 0){
        qDebug() << "관리자 코드가 일치하지 않습니다.\n";
        Popup* popup = new Popup(this, "관리자 코드가 일치하지 않습니다.");
        popup->show();
    } else {
        qDebug() << "관리자 코드 일치 확인\n";

        // 서버 진입
        Server* server = new Server();
        server->show();
        this->close();
    }
}

