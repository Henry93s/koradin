#include "join.h"
#include "ui_join.h"
#include "userInfo.h"
#include "mainwindow.h"
#include "popup.h"

Join::Join(QMainWindow *parent)
    : QWidget(parent)
    , ui(new Ui::Join)
    , parentWindow(parent)
{
    ui->setupUi(this);
    // usermanager 싱글턴 인스턴스 getinstance(ID 중복 검사 및 회원 가입 처리 로직에 필요)
    this->userManagerInstance = this->userManagerInstance->getInstance();
}

Join::~Join()
{
    delete ui;
}

// 회원 가입 시 ID 중복 검사 로직
QString Join::duplicateCheck(){
    if(ui->id_lineEdit->text() == nullptr || ui->name_lineEdit->text() == nullptr ||
        ui->pw_lineEdit->text() == nullptr || ui->email_lineEdit->text() == nullptr){
        Popup* popup = new Popup(this, "입력되지 않은 값이 있습니다.");
        popup->show();
        return "입력되지 않은 값이 있음";
    }

    UserInfo* findUser = userManagerInstance->userSearchById(ui->id_lineEdit->text());

    if(!findUser){
        return "OK";
    } else {
        Popup* popup = new Popup(this, "중복된 아이디가 있습니다.");
        popup->show();
        return "duplicate_ID";
    }
}


void Join::on_join_button_clicked()
{
    // 패스워드, 패스워드 확인 일치 여부 확인
    if(ui->pw_lineEdit->text().compare(ui->pwchk_lineEdit->text()) != 0){
        qDebug() << "패스워드 확인이 일치하지 않습니다.";
        Popup* popup = new Popup(this, "패스워드 확인이 일치하지 않습니다.");
        popup->show();
        return;
    }

    // ID 중복 검사 확인
    QString duplicateResult = duplicateCheck();
    if(duplicateResult.compare("OK") == 0){
        UserInfo* newUser = new UserInfo(ui->id_lineEdit->text(), ui->name_lineEdit->text(),
                                         ui->pw_lineEdit->text(), ui->email_lineEdit->text(), "false");
        userManagerInstance->userInsert(newUser);
    } else {
        qDebug() << duplicateResult << " return this function\n";
        return;
    }

    // userList json update and read
    userManagerInstance->userListJsonSave();
    userManagerInstance->userListJsonLoad();

    // 다시 mainwindow.ui 로 진입
    MainWindow* mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}


void Join::on_cancel_button_clicked()
{
    // 다시 mainwindow.ui 로 진입
    MainWindow* mainwindow = new MainWindow();
    mainwindow->show();
    this->close();
}

