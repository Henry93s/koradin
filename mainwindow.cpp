#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "join.h"
#include "server.h"

#include <QTabWidget>
#include <QFile>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), tabWidget(new QTabWidget(this))
{
    ui->setupUi(this);

    this->usermanager = this->usermanager->getInstance();

    connect(ui->admin_button, &QPushButton::clicked, this, &MainWindow::on_code_Typed);
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
    auto socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", PORT);

    connect(socket, &QTcpSocket::errorOccurred, [=](QAbstractSocket::SocketError) {
        QMessageBox::critical(this, tr("Communication"), tr("연결 실패!"));
    });

    connect(socket, &QTcpSocket::connected, [=]() {
        Client* clientWindow = new Client();
        clientWindow->Initialize(socket);
        clientWindow->show();

        this->close();
    });
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
    Join* joinWindow = new Join();
    joinWindow->show();

    // 창을 완전히 닫지는 않지만 hide() 로 숨김 처리 -> 회원 가입 취소 또는 완료 후 다시 로그인 화면으로 돌아옴
    this->hide();
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

