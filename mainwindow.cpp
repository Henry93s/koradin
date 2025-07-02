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
}

MainWindow::~MainWindow()
{
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
    this->close();
}

