#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "join.h"

#include <QTabWidget>

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

