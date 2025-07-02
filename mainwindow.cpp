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

    this->usermanager = this->usermanager->getInstance();
}

MainWindow::~MainWindow()
{
    // main 프로그램 종료 시 usermanager 소멸자를 통해 현재 userlist 를 userlist.json 에 저장
    delete this->usermanager;
    delete ui;
}

void MainWindow::on_login_button_clicked()
{
    // server 가 open 됨을 확인하고, 로그인 검증이 통과될 경우 현재 mainwindow를 닫고 client.ui 를 연다.
    // client.ui 가 열렸을 때, server connect 를 통한 새 클라이언트 연결 추가.
    Client* clientWindow = new Client();
    clientWindow->show();

    this->close();
}


void MainWindow::on_join_button_clicked()
{
    // 회원 가입 버튼 클릭 시 열리는 회원 가입 화면
    Join* joinWindow = new Join();
    joinWindow->show();

    // 창을 완전히 닫지는 않지만 hide() 로 숨김 처리 -> 회원 가입 취소 또는 완료 후 다시 로그인 화면으로 돌아옴
    this->hide();
}

